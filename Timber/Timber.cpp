#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    sf::CircleShape shape(100.f);
    sf::CircleShape shapeTwo(50.f);
    shape.setFillColor(sf::Color::Blue);
    shapeTwo.setFillColor(sf::Color::Cyan);
    
    // Create a texture for our background and load it.
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    
    // Now create a sprite and attach our background texture to it.
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    
    // Give the spritebackground a position.
    spriteBackground.setPosition(0,0);
    

    while (window.isOpen())
    {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			// left key is pressed: move our character
			window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		// right mouse button is pressed: exit
			window.close();
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(spriteBackground);
        window.draw(shape);window.draw(shapeTwo);
        window.display();
    }

    return 0;
}
