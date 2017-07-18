#!/bin/bash

g++ -std=c++11  -D _WIN32 -I ./socketstream main.cc -lws2_32

