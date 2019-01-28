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

	while (window.isOpen()) 
	{
		// Handle player input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		// Update Scene

		//Draw scene

		// Clear everything from last frame
		window.clear();

		// Draw our game scene
		window.draw(spriteBackground);
		// Show everything we drew
		window.display();
	}


	return 0;
}

