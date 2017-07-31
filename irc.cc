#include "irc.hh"

void irc::message::clear()
{
	origin.nickname.clear();
	origin.user.clear();
	origin.host.clear();
	command.clear();
	params.clear();
}

void irc::message::str(const std::string& s)
{
	std::stringstream sstream;
	std::string prefix, param;

	clear();
	if (s.empty()) return;
	sstream.str(s);
	if (sstream.peek() == ':') {
		sstream.ignore(1);
		sstream >> prefix;
		if (prefix.find_first_of("!@") != std::string::npos) {
			std::stringstream prefixstream;

			prefixstream.str(prefix);
			std::getline(prefixstream, origin.nickname, '!');
			std::getline(prefixstream, origin.user, '@');
			std::getline(prefixstream, origin.host);
		} else {
			origin.host = prefix;
		}
	}
	sstream >> command;
	while (true) {
		sstream >> std::ws;
		if (sstream.peek() == ':') {
			sstream.ignore(1);
			std::getline(sstream, param, '\r');
			params.push_back(param);
			break;
		} else {
			sstream >> param;
			if (!sstream) break;
			params.push_back(param);
		}
	}
}

std::string irc::message::str() const
{
	std::string result;
	std::size_t i;

	if (!origin.nickname.empty()) {
		result += ':';
		result += origin.nickname;
		result += '!';
		result += origin.user;
		result += '@';
		result += origin.host;
		result += ' ';
	} else if (!origin.host.empty()) {
		result += ':';
		result += origin.host;
		result += ' ';
	}
	result += command;
	result += ' ';
	if (!params.empty()) {
		for (i = 0; i < params.size() - 1; ++i) {
			result += params[i];
			result += ' ';
		}
		result += ':';
		result += params[i];
	}
	return result;
}

irc::client::client() : stream(), param()
{
}

irc::client::client(const param_type& p) : stream(), param()
{
	open(p);	
}

irc::client::~client()
{
	close();
}

const irc::client::param_type* irc::client::rdparam() const
{
	return &param;
}

swoope::socketstream* irc::client::rdstream()
{
	return &stream;
}

bool irc::client::is_open() const
{
	return stream.is_open();
}

void irc::client::open(const param_type& p)
{
	std::string line;
	irc::message msg;
	const int reg_complete = 10;
	int reg_progress = 0;

	stream.clear();
	stream.open(p.host, p.port);
	if (!stream) return;
	stream.rdbuf()->pubsetbuf(0, 1024);
	stream.unsetf(std::ios_base::unitbuf);
	if (!p.password.empty())
		stream << "PASS " << p.password << "\r\n";
	stream <<
		"NICK " << p.nickname << "\r\n" <<
		"USER " << p.nickname << " 0 * :" << p.nickname << "\r\n";
	stream.flush();
	if (!stream) {
		stream.close();
		return;
	}
	while (std::getline(stream, line)) {
		msg.str(line);
		line.erase(--line.end());
		std::cout << line << std::endl;
		if (msg.command == "PING") {
			stream << "PONG " << msg.params.back() << "\r\n";
			stream.flush();
			continue;
		}
		std::size_t number = static_cast<std::size_t>(std::atoi(
							msg.command.c_str()));
		if (number <= 4) {
			reg_progress += number;
			if (reg_progress == reg_complete) break;
		} else if (number >= 400) {
			stream.setstate(std::ios_base::failbit);
			break;
		}
	}
	if (!stream) stream.close();
	else param = p;
}

bool irc::client::read(irc::message& msg)
{
	std::string line;
	
	if (is_open() == false) return false;
	std::getline(stream, line);
	if (!stream) return false;
	msg.str(line);
	return true;
}

void irc::client::write(const irc::message& msg)
{
	if (is_open() == false) return;
	stream << msg.str() << "\r\n" << std::flush;
}

void irc::client::shutdown(std::ios_base::openmode how)
{
	stream.shutdown(how);
}

void irc::client::close()
{
	stream.close();
}

bool irc::client::operator!() const
{
	return !stream;
}

