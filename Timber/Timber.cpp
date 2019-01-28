// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    // Create a video mode object
	VideoMode vm(1920, 1080);

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
	float cloud1speed = 0.0f;
	float cloud2speed = 0.0f;
	float cloud3speed = 0.0f;

	Clock clock;

	while (window.isOpen()) 
	{
		// Handle player input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/**** Update Scene *****/
		// Measure time
		Time dt = clock.restart();

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


		/****** Draw scene  *********/

		// Clear everything from last frame
		window.clear();

		// Draw our game scene
		window.draw(spriteBackground);
		// draw clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		// draw tree
		window.draw(spriteTree);
		// draw bee
		window.draw(spriteBee);

		// Show everything we drew
		window.display();
	}


	return 0;
}

