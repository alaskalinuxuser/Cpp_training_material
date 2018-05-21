#!/bin/bash

# You need libsfml installed on your system!

# std=c++1z is for the 2017 standard of C++.

# This will compile under std=c++14 (2014) or std=c++11 (2011) standards as well.
# If your compiler doesn't recognize the std=c++1z, you can try one of the above instead.
# If your compiler is newer, you could try std=c++17 as well.

# My compiler version:
#	alaskalinuxuser@alaskalinuxuser-OptiPlex-7010:~$ g++ --version
#	g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609
#	Copyright (C) 2015 Free Software Foundation, Inc.
#	This is free software; see the source for copying conditions.  There is NO
#	warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

g++ -I./  -Wall -Wextra -g -c -std=c++1z TextureHolder.cpp -o TextureHolder.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z Player.cpp -o Player.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z Arrow.cpp -o Arrow.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z Jello.cpp -o Jello.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z Pickup.cpp -o Pickup.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z CreateHorde.cpp -o CreateHorde.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z CreateBackground.cpp -o CreateBackground.o
g++ -I./  -Wall -Wextra -g -c -std=c++1z JelloStorm.cpp -o JelloStorm.o

g++ JelloStorm.o Player.o Jello.o CreateHorde.o CreateBackground.o TextureHolder.o Arrow.o Pickup.o -o JelloStorm-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./JelloStorm-app

echo "type ./JelloStorm-app to launch"
exit
