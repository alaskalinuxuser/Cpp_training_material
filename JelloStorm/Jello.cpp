#include <sstream>
#include <Jello.h>
#include <TextureHolder.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


void Jello::spawn(float startX, float startY, int type, int seed)
{

	switch (type)
	{
	case 0:
		// Red Jello
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/red_jello.png"));

		m_Speed = 40;
		m_Health = 5;
		break;

	case 1:
		// Blue Jello
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/blue_jello.png"));

		m_Speed = 70;
		m_Health = 1;
		break;

	case 2:
		// Green Jello
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/green_jello.png"));

		m_Speed = 20;
		m_Health = 3;
		break;
	}

	// Modify the speed to make the Jello unique
	// Every jello is unique. Create a speed modifier
	srand((int)time(0) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	// Express as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier;

	m_Position.x = startX;
	m_Position.y = startY;

	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

bool Jello::hit()
{
	m_Health--;

	if (m_Health < 0)
	{
		// dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture(
			"graphics/smearedjello.png"));

		return true;
	}

	// injured but not dead yet
	return false;
}

bool Jello::isAlive()
{
	return m_Alive;
}

FloatRect Jello::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Jello::getCoordPosition()
{
	return m_Sprite.getPosition();
}

Sprite Jello::getSprite()
{
	return m_Sprite;
}

void Jello::update(float elapsedTime,
	Vector2f playerLocation, int specificJello, IntRect arena)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// Update the jello position variables
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x +
			m_Speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y +
			m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x -
			m_Speed * elapsedTime;
	}

	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y -
			m_Speed * elapsedTime;
	}
	
	if (specificJello % 2 == 0) {
		// Give the jello an opportunity to randomly teleport!
		// The if statement checks if it is even number.
		// If it is, do this:
		
		// Find the min/max of the arena area
		int maxY = arena.height - 50;
		int minY = arena.top + 50;
		int maxX = arena.width - 50;
		int minX = arena.left + 50;
	
		// Generate our random number
		srand((int)time(0) * specificJello);
		int side = (rand() % 10);
				
		switch (side)
		{
		case 0:
			// left
			m_Position.x = (rand() % maxX) + minX;
			m_Position.y = (rand() % maxY) + minY;
			break;
		}
	
	}

	// Move the sprite
	m_Sprite.setPosition(m_Position);

	// Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y,
		playerX - m_Position.x)
		* 180) / 3.141;

	m_Sprite.setRotation(angle);


}
