# JelloStorm

![ScreenShot](https://github.com/alaskalinuxuser/Cpp_training_material/blob/master/JelloStorm/graphics/sample.png)

# Gameplay

You are an archer who was traveling through a grassy plain. Suddenly, a horde of ferocious jello monsters storm you! Eat grapes to preserve your strength! Find more arrows to shoot at your enemy!

wasd - move the archer.
r - reload the quiver with arrows.
ESC - exit the game.
ENTER - Pause/resume.
Mouse Right click - shoot an arrow.
Number - Input durring level up.


# Background info
A directory of my work while participating in a class about learning how to program in C++.
All of my work is licensed under the Apache2.0, however, some of the class artwork and sounds may not be opensourced.

To build these games, you need libsfml installed on your Linux system. Then, in each game folder, run the ./build.sh file to build the source.

#A note about the build.sh script

std=c++1z is for the 2017 standard of C++.

This will compile under std=c++14 (2014) or std=c++11 (2011) standards as well.
If your compiler doesn't recognize the std=c++1z, you can try one of the above instead.
If your compiler is newer, you could try std=c++17 as well.

My compiler version:
	alaskalinuxuser@alaskalinuxuser-OptiPlex-7010:~$ g++ --version
	g++ (Ubuntu 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609
	Copyright (C) 2015 Free Software Foundation, Inc.
	This is free software; see the source for copying conditions.  There is NO
	warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#Installation

Pre-reqs: libsfml

If you are reading this, then you have downloaded and extraxted the package already. All you must do now is:

$ ./build.sh
