#!/bin/bash

# You need libsfml installed on your system!

g++ -c -std=c++11 Timber.cpp
g++ Timber.o -o timber-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./timber-app

echo "type ./timber-app to launch"
exit
