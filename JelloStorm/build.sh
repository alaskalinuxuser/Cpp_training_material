#!/bin/bash

# You need libsfml installed on your system!

# This will compile under std=c++1z (2017), std=c++14 (2014), or std=c++11 (2011) standards as well.
# If your compiler doesn't recognize the std=c++1z, you can try one of the above instead.
# If your compiler is newer, you could try std=c++17 as well.

# My compiler version:
#	alaskalinuxuser@alaskalinuxuser-OptiPlex-7010:~$ g++ --version
#	g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609
#	Copyright (C) 2015 Free Software Foundation, Inc.
#	This is free software; see the source for copying conditions.  There is NO
#	warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 TextureHolder.cpp -o TextureHolder.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 Player.cpp -o Player.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 Arrow.cpp -o Arrow.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 Jello.cpp -o Jello.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 Pickup.cpp -o Pickup.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 JelloStorm.cpp -o JelloStorm.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 CreateHorde.cpp -o CreateHorde.o
g++ -I./  -Wall -ansi -Wextra -g -c -std=c++14 CreateBackground.cpp -o CreateBackground.o


g++ TextureHolder.o Player.o Arrow.o Jello.o Pickup.o JelloStorm.o  CreateHorde.o CreateBackground.o -o JelloStorm-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

./JelloStorm-app

echo "type ./JelloStorm-app to launch"
exit
