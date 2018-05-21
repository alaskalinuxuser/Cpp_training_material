#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>
#include <Player.h>
#include <JelloStorm.h>
#include <TextureHolder.h>
#include <Arrow.h>
#include <cmath>

// Declare the functions....

using namespace sf;

// The main process....
int main() {
	
	TextureHolder holder;
	
	// What is the game state?
	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};
	
	// We will start with the game over state.
	State state = State::GAME_OVER; 
	
	// Find the screen resolution
	Vector2f resolution;
	//resolution.x = 800;
	//resolution.y = 600;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	
	// And use it to create an SFML window
	RenderWindow window(VideoMode(resolution.x, resolution.y), 
		"JelloStorm!", Style::Fullscreen);
		
	// Create a an SFML View for the main action
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	// Time control
    Clock clock;

	// Total runtime of Playing state.
	Time gameTimeTotal;

	// Where is the mouse in relation to the virtual world
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to the real screen
	Vector2i mouseScreenPosition;
	
	// Create our player.
	Player player;
	
	// Create an arena, or game area.
	IntRect arena;
	
	// Create our background tiles
	VertexArray tileBackground;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	// Time to make jello!
	int numJellos;
	int numJellosAlive;
	Jello* jellos = nullptr;
	
	// 100 Arrows should do
	Arrow arrows[100];
	int currentArrow = 0;
	int arrowsSpare = 24;
	int arrowsInQuiver = 6;
	int quiverSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;

    while (window.isOpen()) {
		
		
		/* 
		 * 
		 *  Detect input.
		 * 
		 */
		
        Event event;        
        while (window.pollEvent(event)) {
			
			// If they close the window....
			if (event.type == Event::Closed) {
					window.close();
                }
			
			if (event.type == Event::KeyPressed) {									
				
				// Pause game.
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Resume playing.
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R)
					{
						if (arrowsSpare >= quiverSize)
						{
							// Plenty of bullets. Reload.
							arrowsInQuiver = quiverSize;
							arrowsSpare -= quiverSize;
						}
						else if (arrowsSpare > 0)
						{
							// Only few bullets left
							arrowsInQuiver = arrowsSpare;
							arrowsSpare = 0;
						}
						else
						{
							// play a sound...
						}
				}
			}

			}
		}// End event polling
		
		// Escape to quit
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			
			// Fire an arrow
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && arrowsInQuiver > 0)
				{

					// Pass the centre of the player 
					// and the centre of the cross-hair
					// to the shoot function
					arrows[currentArrow].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentArrow++;
					if (currentArrow > 99)
					{
						currentArrow = 0;
					}
					lastPressed = gameTimeTotal;

					arrowsInQuiver--;
				}

			}// End fire an arrow.

		}// End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}
			
			if (state == State::PLAYING)
			{			
				// Prepare the next level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				// We will modify this line of code later
				int tileSize = createBackground(tileBackground, arena);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);
				
				// Make some Jello!
				numJellos = 10;

				// Delete the previously allocated memory (if it exists)
				delete[] jellos;
				jellos = createHorde(numJellos, arena);
				numJellosAlive = numJellos;
				
				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End levelling up
        
        /* 
		 * 
		 *  Updating the scene.
		 * 
		 */
        
        if (state == State::PLAYING) {
			// Update the delta time
			Time dt = clock.restart();
			
			// Update the total game time
			gameTimeTotal += dt;
			
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter());
			
			// Loop through each Jello and update them
			for (int i = 0; i < numJellos; i++)
			{
				if (jellos[i].isAlive())
				{
					jellos[i].update(dt.asSeconds(), playerPosition);
				}
			}
			
			// Loop through any arrows that are in flight.
			for (int i = 0; i < 100; i++)
			{
				if (arrows[i].isInFlight())
				{
					arrows[i].update(dtAsSeconds);
				}
			}
			
		}// End updating the scene
		
		
		
		/* 
		 * 
		 *  Draw the scene.
		 * 
		 */		
		
		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);
			
			window.draw(tileBackground, &textureBackground);
			
			// Draw the Jello!
			for (int i = 0; i < numJellos; i++)
			{
				window.draw(jellos[i].getSprite());
			}
			
			// Draw the arrows.
			for (int i = 0; i < 100; i++)
			{
				if (arrows[i].isInFlight())
				{
					window.draw(arrows[i].getShape());
				}
			}

			// Draw the player
			window.draw(player.getSprite());
		}

		if (state == State::LEVELING_UP)
		{
			//window.clear();
		}

		if (state == State::PAUSED)
		{
			//window.clear();
		}

		if (state == State::GAME_OVER)
		{
			//window.clear();
		}

		window.display();
		
    }
	
	// Delete the previously allocated memory (if it exists)
	delete[] jellos; // Clean up is always a good idea.
    
    return 0;
}
