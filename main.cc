#include "irc.hh"
#include <iostream>

/*
 * main.cc
 * Author: Mark Swoope
 * Date: July 2017
 */

int main(int argc, char* argv[])
{
	irc::client client;
	irc::client::param_type params;
	irc::message rmsg, wmsg;

	params.host = argv[1];
	params.port = argv[2];
	params.nickname = argv[3];
	params.owner = argv[4];
	std::cout << "Connecting..." << std::endl;
	client.open(params);
	if (!client.is_open()) {
		std::cout << "Failed to connect." << std::endl;
		return 1;
	}
	std::cout << "Connection established." << std::endl;
	wmsg.clear();
	wmsg.command = "JOIN";
	wmsg.params.push_back(argv[5]);
	client.write(wmsg);
	while (client.read(rmsg)) {
		std::cout << rmsg.str() << std::endl;
		if (rmsg.command == "PING") {
			wmsg.clear();
			wmsg.command = "PONG";
			wmsg.params.push_back(rmsg.params.back());
			client.write(wmsg);
		} else if (rmsg.command == "PRIVMSG") {
			if (
			rmsg.params.front() == client.rdparam()->nickname &&
			rmsg.origin.nickname == client.rdparam()->owner) {
				wmsg.clear();
				wmsg.str(rmsg.params.back());
				client.write(wmsg);
				if (wmsg.command == "QUIT")
					client.shutdown();
			}
		}
	}
	return 0;
}

