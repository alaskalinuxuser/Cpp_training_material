#pragma once

#include <Jello.h>

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);

Jello* createHorde(int numJellos, IntRect arena);
