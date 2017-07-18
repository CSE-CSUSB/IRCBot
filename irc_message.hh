#ifndef IRC_MESSAGE_HH
#define IRC_MESSAGE_HH
#include <string>
#include <vector>
#include <sstream>

struct irc_message {
	struct {
		std::string nickname;
		std::string user;
		std::string host;
	} origin;
	std::string command;
	std::vector<std::string> params;

	static irc_message from_string(const std::string& s);
};

#endif
