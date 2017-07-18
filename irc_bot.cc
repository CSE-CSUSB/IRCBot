#include "irc_bot.hh"
#include <algorithm>
#include <cctype>

void irc_bot::connect(const std::string& node, const std::string& service,
						const std::string& pass)
{
	socket_traits::socket_type socket;
	irc_message msg;
	int reg_progress{0}, reg_complete{10};

	if (stream_.valid() || nickname_.empty() || owner_.empty()) return;
	socket = make_connected_socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP,
							node, service);
	if (socket == socket_traits::invalid()) return;
	stream_.socket(socket);
	if (!stream_.valid()) return;
	if (!pass.empty())
		stream_ << "PASS " << pass << crlf;
	stream_ <<
		"NICK " << nickname_ << crlf <<
		"USER " << nickname_ << " 0 * :" << nickname_ << crlf;
	while (readmsg(msg)) {
		if (msg.command == "PING") {
			pong(msg.params.back());
		} else if (std::all_of(msg.command.begin(), msg.command.end(),
					[&](char c) -> bool
					{ 
						return std::isdigit(c);
					})) {
			std::size_t number{std::stoul(msg.command)};
			if (number <= 4) {
				reg_progress += number;
				if (reg_progress == reg_complete)
					break;
			} else if (number >= 400) {
				stream_.setstate(std::ios_base::failbit);
				break;
			}
		}
	}
	if (!stream_) stream_.close();
}

void irc_bot::join(const std::string& channel)
{
	if (!stream_.valid()) return;
	stream_ << "JOIN " << channel << crlf;
	channels_.insert(channel);
}

std::set<std::string> irc_bot::names(const std::string& channel)
{
	std::set<std::string> result;
	irc_message msg;
	std::size_t number;

	if (!stream_.valid()) return result;
	stream_ << "NAMES " << channel << crlf;
	while (readmsg(msg)) {
		if (msg.command == "PING") {
			pong(msg.params.back());
			continue;
		}
		number = std::stoul(msg.command);
		if (number == 366 || number >= 400) {
			break;
		} else if (number == 353) {
			std::stringstream names{msg.params.back()};
			std::string n;
			while (names >> n) result.insert(n);
		}
	}
	return result;
}

void irc_bot::nickname(const std::string& set_nickname)
{
	nickname_ = set_nickname;
	if (stream_.valid())
		stream_ << "NICK " << set_nickname << crlf;
}

void irc_bot::part(const std::string& channel, const std::string& msg)
{
	if (!stream_.valid()) return;
	if (msg.empty())
		stream_ << "PART " << channel << crlf;
	else
		stream_ << "PART " << channel << " :" << msg << crlf;
	channels_.erase(channel);
}

void irc_bot::pong(const std::string& id)
{
	if (!stream_.valid()) return;
	stream_ << "PONG " << id << crlf;
}

void irc_bot::privmsg(const std::string& target, const std::string& msg)
{
	if (!stream_.valid()) return;
	stream_ << "PRIVMSG " << target  << " :" << msg << crlf;
}

irc_bot& irc_bot::readmsg(irc_message& msg)
{
	std::string line;

	if (!stream_.valid()) return *this;
	if (!std::getline(stream_, line)) {
		stream_.close();
		node_.clear();
		service_.clear();
		return *this;
	}
	msg = std::move(irc_message::from_string(line));
	return *this;
}

void irc_bot::quit(const std::string& msg)
{
	if (!stream_.valid()) return;
	if (msg.empty())
		stream_ << "QUIT" << crlf;
	else
		stream_ << "QUIT :" << msg << crlf;
	stream_.close();
	node_.clear();
	service_.clear();
}