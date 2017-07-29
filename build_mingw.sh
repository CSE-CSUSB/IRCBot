#!/bin/bash

g++ -std=c++11  -D _WIN32 -o main.exe main.cc irc.cc -lws2_32

