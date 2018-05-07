#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>

// Declare the functions....

// The main process....
int main()
{
	int sWidth = 1024;
	int sHeight = 786;
    sf::RenderWindow window(sf::VideoMode(sWidth, sHeight), "JelloStorm!");
    
    // Create a texture for our background and load it.
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0,0);
    
    // Time control
    sf::Clock clock;
    
    // Add a font.
    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Add startText
    sf::Text startText;
    startText.setString("Press the Enter key to start!");
    startText.setCharacterSize(40);
    startText.setColor(sf::Color::White);
    startText.setFont(font);
    // Set position based on center
    sf::FloatRect textRect = startText.getLocalBounds();
    startText.setOrigin(textRect.left + textRect.width / 2.0f,
    textRect.top + textRect.height / 2.0f);
    startText.setPosition(sWidth / 2.0f,sHeight / 2.0f);
    
    // Add scoreText
    int scoreInt = 0;
    sf::Text scoreText;
    scoreText.setString("Score = 0");
    scoreText.setCharacterSize(40);
    scoreText.setColor(sf::Color::White);
    scoreText.setFont(font);
    scoreText.setPosition(10,10);    
    
    // Accept keyboard input.
    bool acceptInput = false;    
    
	// Prepare the sound
	sf::SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	sf::Sound chop;
	chop.setBuffer(chopBuffer);

    while (window.isOpen()) {
		
		
		/* 
		 * 
		 *  Detect input.
		 * 
		 */
		 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			// Escape key is pressed: Exit the window.
			window.close();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			// Return key is pressed: pause/unpause.
			
			scoreInt = 0;
			timeRemains = 6.0f;
			gamePaused=false;
			
			// Enable keyboard input.
			acceptInput = true;
		}

		if (acceptInput) {
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				
				acceptInput = false;

			}
			
			
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			// right mouse button is pressed: exit
			window.close();
		}
		
        sf::Event event;
        while (window.pollEvent(event)) {
			
			if (event.type == sf::Event::KeyReleased && !gamePaused) {
				// Listen for key presses again
				acceptInput = true;

			} else if (event.type == sf::Event::Closed) {
					window.close();
                }
        }
        
        /* 
		 * 
		 *  Updating the scene.
		 * 
		 */
        
        if (!gamePaused) {
			sf::Time dt = clock.restart();
			
		}
		
		/* 
		 * 
		 *  Draw the scene.
		 * 
		 */		
		
		window.clear();
		window.draw(spriteBackground);
		
		if (gamePaused) {
			window.draw(startText);
		}
		
		window.display();
		
    }

    return 0;
}
