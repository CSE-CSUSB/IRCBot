#ifndef IRC_HH
#define IRC_HH

/*
 * irc.hh
 * Author: Mark Swoope
 * Date: July 2017
 */

#include "socketstream/socketstream.hh"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

struct irc {

	struct message {
		struct {
			std::string
				nickname,
				user, 
				host;
		} origin;
		std::string command;
		std::vector<std::string> params;

		void clear();
		void str(const std::string& s);
		std::string str() const;
	};

	class client {
	public:
		struct param_type {
			std::string
				host,
				port,
				nickname,
				owner,
				password;
		};
	
		client();
		explicit client(const param_type& p);
		virtual ~client();

		const param_type* rdparam() const;
		swoope::socketstream* rdstream();
		bool is_open() const;
		void open(const param_type& p);
		bool read(message& msg);
		void write(const message& msg);
		void shutdown(std::ios_base::openmode how =
					std::ios_base::out);
		void close();
		bool operator!() const;
	private:
		swoope::socketstream stream;
		param_type param;
	};

};

#endif
