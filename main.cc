#include "irc_bot.hh"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	irc_bot bot;
	irc_message msg;

	if (argc != 6) {
		cout << 
			"CSE-CLUB IRC BOT" << endl <<
			"Parameters: HOST PORT NICK OWNER CHANNEL" << endl <<
			endl;
		return 0;
	}
	sockets_init();
	bot.nickname(argv[3]);
	bot.owner(argv[4]);
	cout << "Connecting..." << endl;
	bot.connect(argv[1], argv[2]);
	if (!bot) {
		cerr << "Failed to connect." << endl;
		sockets_quit();
		return 1;
	}
	cout << "Connection established." << endl;
	bot.join(argv[5]);
	while (bot.readmsg(msg)) {
		if (msg.command == "PING") {
			bot.pong(msg.params.back());
		} else if (msg.command == "PRIVMSG") {
			cout << msg.origin.nickname << " -> " <<
						msg.params.front() << 
						" : " << msg.params.back() <<
						endl;
			if (msg.origin.nickname == bot.owner() &&
					msg.params.front() == bot.nickname()) {
				bot.stream() << msg.params.back() << crlf;
			}
		}
		
	}
	sockets_quit();
	return 0;
}
