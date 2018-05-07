#!/bin/bash

# You need libsfml installed on your system!

g++ -c -std=c++11 JelloStorm.cpp
g++ JelloStorm.o -o JelloStorm-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./JelloStorm-app

echo "type ./JelloStorm-app to launch"
exit
