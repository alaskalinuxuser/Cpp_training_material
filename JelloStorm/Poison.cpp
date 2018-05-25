#include <sstream>
#include <Poison.h>
#include <TextureHolder.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


void Poison::spawn(Vector2f startVect)
{

		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/poison.png"));

	m_Position = startVect;

	m_Sprite.setOrigin(50, 50);
	m_Sprite.setPosition(m_Position);
}

bool Poison::isVis()
{
	return m_Vis;
}

FloatRect Poison::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Poison::getSprite()
{
	return m_Sprite;
}

void Poison::update(Vector2f updateVect)
{
	m_Position = updateVect;
	// Move the sprite
	m_Sprite.setPosition(m_Position);
}
