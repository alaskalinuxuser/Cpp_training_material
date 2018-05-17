#!/bin/bash

# You need libsfml installed on your system!

g++ -I./  -Wall -Wextra -g -c -std=c++11 Player.cpp -o Player.o
g++ -I./  -Wall -Wextra -g -c -std=c++11 CreateBackground.cpp -o CreateBackground.o
g++ -I./  -Wall -Wextra -g -c -std=c++11 JelloStorm.cpp -o JelloStorm.o

g++ JelloStorm.o Player.o CreateBackground.o -o JelloStorm-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./JelloStorm-app

echo "type ./JelloStorm-app to launch"
exit
