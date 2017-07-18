#!/bin/bash

g++ -std=c++11  -D _WIN32 -I ./socketstream main.cc irc_bot.cc irc_message.cc -lws2_32

