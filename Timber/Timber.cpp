#include <SFML/Graphics.hpp>
#include <sstream>

// Declare the function...
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];
enum class sideClass { LEFT, RIGHT, NONE };
sideClass branchPositions[NUM_BRANCHES];

int main()
{
	int sWidth = 1024;
	int sHeight = 786;
    sf::RenderWindow window(sf::VideoMode(sWidth, sHeight), "Timber!");
    
    // Create a texture for our background and load it.
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    
    // Now create a sprite and attach our background texture to it.
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    
    // Give the spritebackground a position.
    spriteBackground.setPosition(0,0);
    
    // Add our tree
    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(sWidth / 2.0f - 150,0);
    
    // Add some clouds
    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    sf::Sprite spriteCloudOne;
    sf::Sprite spriteCloudTwo;
    sf::Sprite spriteCloudThree;
    spriteCloudOne.setTexture(textureCloud);
    spriteCloudTwo.setTexture(textureCloud);
    spriteCloudThree.setTexture(textureCloud);
    spriteCloudOne.setPosition(sWidth + 100,250);
    spriteCloudTwo.setPosition(sWidth + 100,125);
    spriteCloudThree.setPosition(sWidth + 100,0);
    bool cloudOneActive = false;
    bool cloudTwoActive = false;
    bool cloudThreeActive = false;
    float cloudOneSpeed = 0.0f;
    float cloudTwoSpeed = 0.0f;
    float cloudThreeSpeed = 0.0f;
    
    // Add our bee
    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/insect.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(sWidth - 100,600);
    bool beeActive = false;
    float beeSpeed = 0.0f;
    
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
    
    // Add a time bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 40;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth,timeBarHeight));
    timeBar.setFillColor(sf::Color::Blue);
    timeBar.setPosition(sWidth / 2.0f - (timeBarStartWidth / 2.0f),sHeight - 45);
    // Time bar time calculations.
    sf::Time gameTime;
    float timeRemains = 6.0f;
    float widthPerSecond = timeBarStartWidth / timeRemains;
    
    // Add branches.
    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    // For loop to set up branches.
    for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000,2000); // Way off screen.
		branches[i].setOrigin(220,20); // The center of branch.png
	}
    
    // Time control
    sf::Clock clock;
    
    // Pause the game
    bool gamePaused = true;
    
    // Code to just randomly display the branches.
     updateBranches(1); updateBranches(2); updateBranches(3); updateBranches(4); updateBranches(5);

    while (window.isOpen())
    {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			// Escape key is pressed: Exit the window.
			window.close();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			// Return key is pressed: pause/unpause.
			
			scoreInt = 0;
			timeRemains = 6.0f;
			gamePaused=false;
				
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			// right mouse button is pressed: exit
			window.close();
		}
		
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (!gamePaused) {
			sf::Time dt = clock.restart();
			
			// Update the time bar.
			timeRemains -= dt.asSeconds();
			timeBar.setSize(sf::Vector2f(widthPerSecond * timeRemains, timeBarHeight));
			
			if (timeRemains <= 0.0f) {
				gamePaused=true;
				startText.setString("Time Over! Press Enter to play again!");
				
				sf::FloatRect textRect = startText.getLocalBounds();
				startText.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
				startText.setPosition(sWidth / 2.0f,sHeight / 2.0f);
			}
			
			if (!beeActive) {
				// Since the bee is not active.
				// Seed our random number with time from system.
				srand((int)time(0) * 10);
				// Use the random to get a number between 0 and 199.
				beeSpeed = ((rand() % 200) + 200);
				// Now set the bee's starting position.
				srand((int)time(0) * 10);
				float height = ((rand() % (sHeight/2)) + 200);
				spriteBee.setPosition(-100,height); //off screen on left.
				beeActive = true; // Make the bee active, so only runs once.
			} else {
				// Since the bee is active, let's move it.
				spriteBee.setPosition(
				spriteBee.getPosition().x + (beeSpeed * dt.asSeconds()),
				spriteBee.getPosition().y);
			}
			
			if (spriteBee.getPosition().x > sWidth + 100) {
				// The bee is now off screen. Let's move it back.
				beeActive = false;
				// This will then call the setup for the bee again.
			}
			
			if (!cloudOneActive) {
				// Since the Cloud is not active.
				// Seed our random number with time from system.
				srand((int)time(0) * 10);
				// Use the random to get a number between 0 and 199.
				cloudOneSpeed = ((rand() % 150) + 50);
				// Now set the Clouds's starting position.
				srand((int)time(0) * 10);
				float height = ((rand() % 300) + 10);
				spriteCloudOne.setPosition(sWidth + 100,height); //off screen on right.
				cloudOneActive = true; // Make the cloud active, so only runs once.
			} else {
				// Since the Cloud is active, let's move it.
				spriteCloudOne.setPosition(
				spriteCloudOne.getPosition().x - (cloudOneSpeed * dt.asSeconds()),
				spriteCloudOne.getPosition().y);
			}
			
			if (spriteCloudOne.getPosition().x < -100) {
				// The cloud is now off screen. Let's move it back.
				cloudOneActive = false;
				// This will then call the setup for the cloud again.
			}
			
			if (!cloudTwoActive) {
				// Since the Cloud is not active.
				// Seed our random number with time from system.
				srand((int)time(0) * 10);
				// Use the random to get a number between 0 and 199.
				cloudTwoSpeed = ((rand() % 150) + 60);
				// Now set the Clouds's starting position.
				srand((int)time(0) * 20);
				float height = ((rand() % 200) + 10);
				spriteCloudTwo.setPosition(sWidth + 100,height); //off screen on right.
				cloudTwoActive = true; // Make the cloud active, so only runs once.
			} else {
				// Since the Cloud is active, let's move it.
				spriteCloudTwo.setPosition(
				spriteCloudTwo.getPosition().x - (cloudTwoSpeed * dt.asSeconds()),
				spriteCloudTwo.getPosition().y);
			}
			
			if (spriteCloudTwo.getPosition().x < -100) {
				// The cloud is now off screen. Let's move it back.
				cloudTwoActive = false;
				// This will then call the setup for the cloud again.
			}
			
			if (!cloudThreeActive) {
				// Since the Cloud is not active.
				// Seed our random number with time from system.
				srand((int)time(0) * 30);
				// Use the random to get a number between 0 and 199.
				cloudThreeSpeed = ((rand() % 150) + 40);
				// Now set the Clouds's starting position.
				srand((int)time(0) * 10);
				float height = ((rand() % 250) + 10);
				spriteCloudThree.setPosition(sWidth + 100,height); //off screen on right.
				cloudThreeActive = true; // Make the cloud active, so only runs once.
			} else {
				// Since the Cloud is active, let's move it.
				spriteCloudThree.setPosition(
				spriteCloudThree.getPosition().x - (cloudThreeSpeed * dt.asSeconds()),
				spriteCloudThree.getPosition().y);
			}
			
			if (spriteCloudThree.getPosition().x < -100) {
				// The cloud is now off screen. Let's move it back.
				cloudThreeActive = false;
				// This will then call the setup for the cloud again.
			}
		}
		
		// Update the score.
		std::stringstream ss;
		ss << "Score = " << scoreInt;
		scoreText.setString(ss.str());
		
		// update the branches.
			for (int i = 0; i < NUM_BRANCHES; i++)
			{

				float height = i * 150;

				if (branchPositions[i] == sideClass::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(sWidth / 2.0f - 370, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == sideClass::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(sWidth / 2 + 370, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);

				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}
		
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteCloudOne);
		window.draw(spriteCloudTwo);
		window.draw(spriteCloudThree);
		window.draw(spriteTree);
		
		// Draw all the branches.
		for (int b = 0; b < NUM_BRANCHES; b++) {
			window.draw(branches[b]);
		}
		
		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(timeBar);
		
		if (gamePaused) {
			window.draw(startText);
		}
		
		window.display();
		
    }

    return 0;
}


// The branches function.
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = sideClass::LEFT;
		break;

	case 1:
		branchPositions[0] = sideClass::RIGHT;
		break;

	default:
		branchPositions[0] = sideClass::NONE;
		break;
	}


}

