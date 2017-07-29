#ifndef IRC_HH
#define IRC_HH

#include "socketstream/socketstream.hh"

#include <string>
#include <vector>
#include <sstream>
#include <cctype>

struct irc {
	struct connect_params {
		std::string \
			host,
			port,
			nickname,
			owner,
			password;
	};

	static swoope::socketstream connect(const connect_params& params);

	struct message {
		struct {
			std::string \
				nickname,
				user, 
				host;
		} origin;
		std::string command;
		std::vector<std::string> params;

		static message from_string(const std::string& s);
	};

};

#endif
