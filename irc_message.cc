#include "irc_message.hh"

irc_message irc_message::from_string(const std::string& s)
{
	irc_message result;
	std::string prefix, param;

	if (s.empty()) return {};
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
