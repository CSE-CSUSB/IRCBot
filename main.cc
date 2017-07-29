#include "irc.hh"
#include <iostream>
#include <utility>
using namespace std;


int main(int argc, char* argv[])
{
	swoope::socketstream client;
	irc::message msg;
	string line, nickname, owner;

	if (argc != 6) {
		cout << 
			"CSE-CLUB IRC BOT" << endl <<
			"Parameters: HOST PORT NICK OWNER CHANNEL" << endl <<
			endl;
		return 0;
	}
	nickname = argv[3];
	owner = argv[4];
	cout << "Connecting..." << endl;
	client = std::move(irc::connect(irc::connect_params{argv[1], argv[2],
							nickname, owner, ""}));
	if (!client) {
		cout << "Failed to connect." << endl;
		return 1;
	}
	cout << "Connection established." << endl;
	client << "JOIN " << argv[5] << "\r\n" << flush;
	while (std::getline(client, line)) {
		if (line.back() == '\r') line.pop_back();
		cout << line << endl;
		msg = std::move(irc::message::from_string(line));
		if (msg.command == "PING") {
			client << "PONG " << msg.params.back() << "\r\n" <<
								flush;
		} else if (msg.command == "PRIVMSG") {
			if (msg.origin.nickname == owner &&
					msg.params.front() == nickname) {
				client << msg.params.back() << "\r\n" << flush;
			}
		}
	}
	return 0;
}
