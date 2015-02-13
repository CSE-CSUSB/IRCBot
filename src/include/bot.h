#ifndef BOT_H
#define BOT_H

#include <string>
using namespace std;

class Bot
{
private:
    string server;
    string port;
    int s; //the socket descriptor
    string nick;
    string usr;
    bool isConnected(string buf);
    string  timeNow();
    bool sendData(string msg);
    void sendPong(string buf);
    void msgHandel(string buf);

public:
    Bot(string _nick, string  _usr, string _server = "irc.freenode.net", string _port = "6667");
    virtual ~Bot();

    bool setup;

    void start();
    bool charSearch(string toSearch, string searchFor);
};

#endif