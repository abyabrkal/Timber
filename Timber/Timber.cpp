// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include "pch.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

// Function declarattion
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is player/branch? Left or Right
enum class side { LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];


int main()
{
    // Create a video mode object
	//VideoMode vm(1920, 1080);
	VideoMode vm(1280, 1024);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Create a texture to hold a graphic on GPU
	Texture textureBackground;

	//Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground position to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Prepare the Bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	// is the beee currently moving?
	bool beeActive = false;
	// Hoe fast can bee fly
	float beeSpeed = 0.0f;

	// Set 3 cloud
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1, spriteCloud2, spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// are clouds on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Variables to control time itself
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// track whether game is running
	bool paused = true;

	// Draw some text
	int score = 0;
	Text messageText;
	Text scoreText;

	// we need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	//assign the actual message
	messageText.setString("Press Enter to start");
	scoreText.setString("Score = 0");

	// make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// set the sprite origin to dead center
		// we can then spin it around without changing its position
		branches[i].setOrigin(220, 20);
	}
	
	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// The player starts on the left
	side playerSide = side::LEFT;

	// prepare gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	// prepare sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// death
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for the keypresses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
			   
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// reset the time and score
			score = 0;
			timeRemaining = 5;
			
			// make all branches disapper
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// move player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		// wrap player controls to make sure we areacepting input
		if (acceptInput)
		{
			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right
				playerSide = side::RIGHT;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);



				spritePlayer.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				// stop chopping
				acceptInput = false;

				// play chop sound
				chop.play();
		
			}

			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the left
				playerSide = side::LEFT;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);


				spritePlayer.setPosition(580, 720);

				// update the branches
				updateBranches(score);

				// set the log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				// stop chopping
				acceptInput = false;

				// play chop sound
				chop.play();
			}
		}
		/*
		****************************************
		Update the scene
		****************************************
		*/
		if(!paused)
		{
			// Measure time
			Time dt = clock.restart();

			// subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the timebar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// pause the game
				paused = true;

				// change the message shown to players
				messageText.setString("Out of Time!");

				// reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// play out of time sound
				outOfTime.play();
			}


			// setup bee
			if (!beeActive)
			{
				// how fast is bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// how high is bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;

			}
			else
			{
				// move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// has bee reached right edge
				if (spriteBee.getPosition().x < -100)
				{
					// set it up ready to be a new cloud next frame
					beeActive = false;
				}
			}

			// Manage the clouds
			// Cloud 1
			if (!cloud1Active)
			{

				// How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;


			}
			else
			{

				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud1.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
			}
			// Cloud 2
			if (!cloud2Active)
			{

				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;


			}
			else
			{

				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud2.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}
			}

			if (!cloud3Active)
			{

				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;


			}
			else
			{

				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud3.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			// update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());


			// update branch sprite
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// move the sprite to left side
					branches[i].setPosition(610, height);
					// flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// move the sprite to right side
					branches[i].setPosition(1330, height);
					// flip the sprite round the other way
					branches[i].setRotation(0);
				}
				else
				{
					// hide the branch
					branches[i].setPosition(3000, height);
				}
			}


			// Handle a flying log				
			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the insect reached the right hand edge of the screen?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new cloud next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

				// hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// Play the death sound
				death.play();
			}


		} // End of if(!paused)

		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from last frame
		window.clear();

		// Draw our game scene
		window.draw(spriteBackground);
		// draw clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// draw branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		// draw tree
		window.draw(spriteTree);

		// draw player
		window.draw(spritePlayer);

		// draw axe
		window.draw(spriteAxe);

		// draw flying log
		window.draw(spriteLog);

		// draw gravestone
		window.draw(spriteRIP);

		// draw bee
		window.draw(spriteBee);

		// draw the score
		window.draw(scoreText);

		// draw timebar
		window.draw(timeBar);

		if (paused)
		{
			// draw our message
			window.draw(messageText);
		}

		// Show everything we drew
		window.display();
	}


	return 0;
}


// Function definition: updateBranches()
void updateBranches(int seed)
{
	// move all branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// spawn a new branch at position 0
	// left, right, none
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}

}
