#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>
#include <Player.h>
#include <JelloStorm.h>

// Declare the functions....

using namespace sf;

// The main process....
int main() {
	
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
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");

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

			// Draw the player
			window.draw(player.getSprite());
		}

		if (state == State::LEVELING_UP)
		{
			window.clear();
		}

		if (state == State::PAUSED)
		{
			window.clear();
		}

		if (state == State::GAME_OVER)
		{
			window.clear();
		}

		window.display();
		
    }

    return 0;
}
