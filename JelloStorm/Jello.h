#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

class Jello {
	
private:
	// How fast is each jello type?
	const float RED_SPEED = 40; //Bloater in original game.
	const float BLUE_SPEED = 80; //Chaser in original game.
	const float GREEN_SPEED = 20; //Crawler in original game.

	// How tough is each jello type
	const float RED_HEALTH = 5;
	const float BLUE_HEALTH = 1;
	const float GREEN_HEALTH = 3;

	// Make each jello vary its speed slightly
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	// Where is this jello?
	Vector2f m_Position;

	// A sprite for the jello
	Sprite m_Sprite;

	// How fast can this one run/crawl?
	float m_Speed;

	// How much health has it got?
	float m_Health;

	// Is it still alive?
	bool m_Alive = true;

	// Public prototypes go here	
public:

	// Handle when a bullet hits a jello
	bool hit();

	// Find out if the jello is alive
	bool isAlive();

	// Spawn a new jello
	void spawn(float startX, float startY, int type, int seed);

	// Return a rectangle that is the position in the world
	FloatRect getPosition();
	
	// Return coordinate position
	Vector2f getCoordPosition();

	// Get a copy of the sprite to draw
	Sprite getSprite();

	// Update the jello each frame
	void update(float elapsedTime, Vector2f playerLocation,
	 int specificJello, IntRect arena);
};


