#!/bin/bash

g++ -c Timber.cpp
g++ Timber.o -o timber-app -lsfml-graphics -lsfml-window -lsfml-system

./timber-app

echo "type ./sfml-app to launch"
exit
