#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

class Poison {
	
private:
	
	// Where is this Poison?
	Vector2f m_Position;

	// A sprite for the Poison
	Sprite m_Sprite;

	// Is it visable?
	bool m_Vis = true;

	// Public prototypes go here	
public:

	// Find out if the poison is visable
	bool isVis();

	// Spawn a new Poison
	void spawn(Vector2f startVect);

	// Return a rectangle that is the position in the world
	FloatRect getPosition();

	// Get a copy of the sprite to draw
	Sprite getSprite();

	// Update the poison each frame
	void update(Vector2f updateVect);
};


