#include "irc.hh"
#include <iostream>

swoope::socketstream irc::connect(const irc::connect_params& params)
{
	swoope::socketstream client{}, result{};
	irc::message msg;
	std::string line;
	int reg_progress{0};
	const int reg_complete{10};

	result.setstate(std::ios_base::failbit);
	client.open(params.host, params.port);
	if (!client) return std::move(result);
	client.rdbuf()->pubsetbuf(nullptr, 1024);
	client.unsetf(std::ios_base::unitbuf);
	if (!params.password.empty())
		client << "PASS " << params.password << "\r\n";
	client <<
		"NICK " << params.nickname << "\r\n" <<
		"USER " << params.nickname << " 0 * :" << params.nickname <<
							"\r\n";
	client.flush();
	if (!client) return std::move(result);
	while (std::getline(client, line)) {
		if (line.back() == '\r') line.pop_back();
		std::cout << line << std::endl;
		msg = std::move(irc::message::from_string(line));
		if (msg.command == "PING") {
			client << "PONG " << msg.params.back() << "\r\n" <<
								std::flush;
		} else if (std::all_of(msg.command.begin(), msg.command.end(),
					[&](char c) -> bool
					{ return std::isdigit(c); })) {
			std::size_t number;

			number = static_cast<std::size_t>(std::stoi(
							msg.command));
			if (number <= 4) {
				reg_progress += number;
				if (reg_progress == reg_complete) break;
			} else if (number >= 400) {
				client.setstate(std::ios_base::failbit);
				break;
			}
		}
	}
	if (client) result = std::move(client);
	return std::move(result);
}

irc::message irc::message::from_string(const std::string& s)
{
	irc::message result{};
	std::string prefix, param;

	if (s.empty()) return result;
	std::stringstream ss{s};
	if (ss.peek() == ':') {
		ss.ignore(1);
		ss >> prefix;
		if (prefix.find_first_of("!@") != std::string::npos) {
			std::stringstream pss{prefix};
			std::getline(pss, result.origin.nickname, '!');
			std::getline(pss, result.origin.user, '@');
			pss >> result.origin.host;
		} else {
			result.origin.host = prefix;
		}
	}
	ss >> result.command;
	while (true) {
		ss >> std::ws;
		if (ss.peek() == ':') {
			ss.ignore(1);
			std::getline(ss, param);
			if (param.back() == '\r') param.pop_back();
			result.params.push_back(param);
			break;
		} else {
			if (!(ss >> param)) break;
			result.params.push_back(param);
		}
	}
	return result;
}
