#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

class Arrow
{
private:
	// Where is the arrow?
	Vector2f m_Position;

	// What each arrow looks like
	RectangleShape m_ArrowShape;

	// Is this arrow currently whizzing through the air
	bool m_InFlight = false;

	// How fast does a arrow travel?
	float m_ArrowSpeed = 750;

	// What fraction of 1 pixel does the arrow travel, 
	// Horizontally and vertically each frame?
	// These values will be derived from m_ArrowSpeed
	float m_ArrowDistanceX;
	float m_ArrowDistanceY;

	// Where is this arrow headed to?
	float m_XTarget;
	float m_YTarget;

	// Some boundaries so the arrow doesn't fly forever
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

// Public function prototypes go here
public:
	// The constructor
	Arrow();

	// Stop the arrow
	void stop();

	// Returns the value of m_InFlight
	bool isInFlight();

	// Launch a new arrow
	void shoot(float startX, float startY,
		float xTarget, float yTarget);

	// Tell the calling code where the arrow is in the world
	FloatRect getPosition();

	// Return the actual shape (for drawing)
	RectangleShape getShape();

	// Update the arrow each frame
	void update(float elapsedTime);

};
