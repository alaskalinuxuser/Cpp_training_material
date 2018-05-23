#include <sstream>
#include <Jello.h>
#include <JelloStorm.h>

Jello* createHorde(int numJellos, IntRect arena)
{
	Jello* jellos = new Jello[numJellos];

	int maxY = arena.height + 100;
	int minY = arena.top - 100;
	int maxX = arena.width - 100;
	int minX = arena.left + 100;

	for (int i = 0; i < numJellos; i++)
	{

		// Which side should the jello spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side)
		{
		case 0:
			// left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			// right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			// top
			x = (rand() % maxX) + minX;
			y = minY;
			break;

		case 3:
			// bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}

		// Red, Blue, or Green
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);

		// Spawn the new jello into the array
		jellos[i].spawn(x, y, type, i);

	}
	return jellos;
}
