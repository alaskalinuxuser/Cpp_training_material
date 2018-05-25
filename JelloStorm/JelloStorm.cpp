#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <SFML/Audio.hpp>
#include <Player.h>
#include <Poison.h>
#include <JelloStorm.h>
#include <TextureHolder.h>
#include <Arrow.h>
#include <Pickup.h>
#include <cmath>
#include <fstream>

// Declare the functions....

using namespace sf;

// The main process....
int main() {
	
	TextureHolder holder;
	
	// What is the game state?
	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, DEAD, PLAYING
	};
	
	// We will start with the game over state.
	State state = State::GAME_OVER; 
	
	// Find the screen resolution
	Vector2f resolution;
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
	
	// and Poison!
	Poison* poison = nullptr;
	
	// 100 Arrows should do
	Arrow arrows[100];
	int currentArrow = 0;
	int arrowsSpare = 24;
	int arrowsInQuiver = 6;
	int quiverSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;
	
	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup arrowPickup(2);
	
	// About the game
	int score = 0;
	int hiScore = 0;
	
	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background_stretch.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setScale(resolution.x/341, resolution.y/206);
	spriteGameOver.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a sprite for the arrow icon
	Sprite spriteArrowIcon;
	Texture textureArrowIcon = TextureHolder::GetTexture("graphics/arrow_icon.png");
	spriteArrowIcon.setTexture(textureArrowIcon);
	spriteArrowIcon.setPosition(20, resolution.y - 100);

	// Load the font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(55);
	pausedText.setColor(Color::White);
	pausedText.setPosition(resolution.x/4, resolution.y/4);
	pausedText.setString("Press Enter \nto continue");
	
	//Dead
	Text YouDiedText;
	YouDiedText.setFont(font);
	YouDiedText.setCharacterSize(55);
	YouDiedText.setColor(Color::White);
	YouDiedText.setPosition(resolution.x/4, resolution.y/4);
	YouDiedText.setString("You are dead! \nPress Enter to continue.");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(45);
	gameOverText.setColor(Color::White);
	gameOverText.setPosition(resolution.x/5, resolution.y/4);
	std::stringstream gameOverStream;
	gameOverStream <<
		"Press Enter to play!" <<
		"\nW - Up" <<
		"\nA - Left" <<
		"\nS - Down" <<
		"\nD - Right" <<
		"\nR - Reload quiver from inventory" <<
		"\nEnter - Pause the game" <<
		"\nMouse - Left Click to shoot" <<
		"\n      - Right Click to reload";
	gameOverText.setString(gameOverStream.str());

	// Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(35);
	levelUpText.setColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"Choose an option:" <<
		"\n1 - Faster rate of fire" <<
		"\n2 - Larger quiver size(next reload)" <<
		"\n3 - Increased max health" <<
		"\n4 - Faster running speed" <<
		"\n5 - More and better health pickups" <<
		"\n6 - More and better arrow pickups";
	levelUpText.setString(levelUpStream.str());

	// Arrow
	Text arrowText;
	arrowText.setFont(font);
	arrowText.setCharacterSize(35);
	arrowText.setColor(Color::White);
	arrowText.setPosition(200, resolution.y - 100);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(35);
	scoreText.setColor(Color::White);
	scoreText.setPosition(20, 0);
	
	// Load the high score from a text file/
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(35);
	hiScoreText.setColor(Color::White);
	hiScoreText.setPosition(resolution.x - 300, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Jellos remaining
	Text jellosRemainingText;
	jellosRemainingText.setFont(font);
	jellosRemainingText.setCharacterSize(35);
	jellosRemainingText.setColor(Color::White);
	jellosRemainingText.setPosition(resolution.x - 300, resolution.y - 100);
	jellosRemainingText.setString("Jellos: ");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(35);
	waveNumberText.setColor(Color::White);
	waveNumberText.setPosition(resolution.x - 300, resolution.y - 200);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, resolution.y - 100);
		
	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// What time was the last update
	Time timeSinceLastUpdate;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 50;
	
	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// The main game loop
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
				
				// Restart game.
				else if (event.key.code == Keyboard::Return &&
					state == State::DEAD)
				{
					state = State::GAME_OVER;
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
					wave = 0;
					score = 0;

					// Prepare the stats for next game
					currentArrow = 0;
					arrowsSpare = 24;
					arrowsInQuiver = 6;
					quiverSize = 6;
					fireRate = 1;

					// Reset the player's stats
					player.resetPlayerStats();
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
							reload.play();
						}
						else if (arrowsSpare > 0)
						{
							// Only few bullets left
							arrowsInQuiver = arrowsSpare;
							arrowsSpare = 0;
							reload.play();
						}
						else
						{
							// play a sound...
							reloadFailed.play();
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
			
			// Fire an arrow with mouse click.
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
					shoot.play();
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
				// Increase fire rate
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// Increase clip size
				quiverSize += quiverSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// Increase health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// Increase speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				arrowPickup.upgrade();
				state = State::PLAYING;
			}
			
			if (state == State::PLAYING)
			{			
				// Increase the wave number
				wave++;
				// Prepare thelevel
				arena.width = 700 + (wave*100);
				arena.height = 700 + (wave*100);
				arena.left = 0;
				arena.top = 0;

				// Set the tile size.
				int tileSize = createBackground(tileBackground, arena);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);
				
				// Configure the pick-ups
				healthPickup.setArena(arena);
				arrowPickup.setArena(arena);

				// Make some Jello!
				numJellos = 5 * wave;

				// Delete the previously allocated memory (if it exists)
				delete[] jellos;
				jellos = createHorde(numJellos, arena);
				
				delete[] poison;
				poison = new Poison[numJellos];
				
				for (int p = 0; p < numJellos; p++)
				{
					// Spawn the new jello into the array
					poison[p].spawn(jellos[p].getCoordPosition());
				}
				 
				 numJellosAlive = numJellos;
				 
				 // A catch all to make sure at least 80% of the jellos were created.
				 // If not, try again.
				 if (!jellos[numJellosAlive - (numJellosAlive/5*4)].isAlive())
				 {
					 delete[] jellos;
					 jellos = createHorde(numJellos, arena);
				 
					 numJellosAlive = numJellos;
				 }
				
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
				
			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter());
			
			// Loop through each Jello and update them
			
			numJellosAlive = 0;
			
			for (int i = 0; i < numJellos; i++)
			{
				if (jellos[i].isAlive())
				{
					jellos[i].update(dt.asSeconds(), playerPosition,
					 i, arena);
					 
					 // And update the poison ring.
					 poison[i].update(jellos[i].getCoordPosition());
					
					// Used for debugging
					// std::cout << i;
					
					numJellosAlive++;
				} else {
					// Since the Jello is dead, remove it's poison ring.
					Vector2f offVect;
					offVect.x = -1000000;
					offVect.y = -1000000;
					poison[i].update(offVect);
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
			
			// Update the pickups
			healthPickup.update(dtAsSeconds);
			arrowPickup.update(dtAsSeconds);
			
			// Collision detections
			// Have any jello been hit with an arrow?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numJellos; j++)
				{
					if (arrows[i].isInFlight() &&
						jellos[j].isAlive())
					{
						if (arrows[i].getPosition().intersects
							(jellos[j].getPosition()))
						{
							// Stop the arrow
							arrows[i].stop();

							// Register the hit and see if it was a kill
							if (jellos[j].hit()) {
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numJellosAlive--;

							}

							// Make a splat sound
							splat.play();
							
						}
					}

				}
			}// End jello been hit by an arrow.
			
			// When all the jellos are dead (again)
								if (numJellosAlive == 0) {
									state = State::LEVELING_UP;
								}

			 // Have any jellos touched the archer?			
			for (int i = 0; i < numJellos; i++)
			{
				if (player.getPosition().intersects
					(jellos[i].getPosition()) && jellos[i].isAlive())
				{

					if (player.hit(gameTimeTotal))
					{
						// Play hit sound
						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::DEAD;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
						
					}
				}
			}// End player touched

			 // Has the player touched health pickup
			if (player.getPosition().intersects
				(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				// Play a sound
				pickup.play();

			}

			// Has the player touched arrow pickup
			if (player.getPosition().intersects
				(arrowPickup.getPosition()) && arrowPickup.isSpawned())
			{
				arrowsSpare += arrowPickup.gotIt();
				// Play a sound
				pickup.play();

			}
			
			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			// Increment the amount of time since the last HUD update
			timeSinceLastUpdate += dt;
			
			// Increment the number of frames since the last HUD calculation
			framesSinceLastHUDUpdate++;
			// Calculate FPS every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{

				// Update game HUD text
				std::stringstream ssArrow;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssJellosAlive;

				// Update the arrow text
				ssArrow << arrowsInQuiver << "/" << arrowsSpare;
				arrowText.setString(ssArrow.str());

				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update the high score text
				ssJellosAlive << "Jellos:" << numJellosAlive;
				jellosRemainingText.setString(ssJellosAlive.str());

				framesSinceLastHUDUpdate = 0;
				timeSinceLastUpdate = Time::Zero;
			}// End HUD update
			
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
			
			// Draw the pick-ups, if currently spawned
			if (arrowPickup.isSpawned())
			{
				window.draw(arrowPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			
			// Draw the Jello!
			for (int i = 0; i < numJellos; i++)
			{
				window.draw(jellos[i].getSprite());
				window.draw(poison[i].getSprite());
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
			
			//Draw the crosshair
			window.draw(spriteCrosshair);
			
			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteArrowIcon);
			window.draw(arrowText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(jellosRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		
		if (state == State::DEAD)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);
			
			window.draw(tileBackground, &textureBackground);
			
			// Draw the pick-ups, if currently spawned
			if (arrowPickup.isSpawned())
			{
				window.draw(arrowPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			
			// Draw the Jello!
			for (int i = 0; i < numJellos; i++)
			{
				window.draw(jellos[i].getSprite());
				window.draw(poison[i].getSprite());
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
			
			//Draw the crosshair
			window.draw(spriteCrosshair);
			
			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteArrowIcon);
			window.draw(arrowText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(jellosRemainingText);
			window.draw(YouDiedText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();
		
    }
    
    return 0;
}
