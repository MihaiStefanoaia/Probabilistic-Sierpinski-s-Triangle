#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "button.h"
#include "textInput.h"
#include <array>

const float pi = 3.14159f;


void fitSpriteToWindow(sf::Sprite& sprite, sf::Window& window) //sets scale of the spirte to fit perfectly inside the screen
{
	sf::Vector2u windowSize = window.getSize(), spriteSize = sprite.getTexture()->getSize();
	sf::Vector2f scl((float)windowSize.x, (float)windowSize.y);
	scl.x /= spriteSize.x;
	scl.y /= spriteSize.y;
	sprite.setScale(scl);
}

int main()
{
	bool exitToken = 0;
	int nrPoints=3;
	sf::Vector2f p, * points;

	while (!exitToken)
	{
		//needs to be modified a ~~bit~~ lot
		sf::Vector2f canvasSize(3000,3000);
		sf::Vector2f windowSize(1000,1000);


		sf::RenderWindow initialConfig(sf::VideoMode(500, 300), "Configuration", sf::Style::Default);

		initialConfig.setFramerateLimit(30);

		button exit(sf::Vector2f(25, 200), sf::Vector2f(200, 100), "Exit", "CC");
		button start(sf::Vector2f(275, 200), sf::Vector2f(200, 100), "Start", "CC");

		textInput canvasSizeInput(sf::Vector2f(0, 0), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Canvas Size (px):");
		textInput windowSizeInput(sf::Vector2f(0, 50), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Window Size (px):");
		textInput pointNumberInput(sf::Vector2f(0, 100), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Number of Points:");

		std::array<textInput*, 3> textInputs = {&canvasSizeInput, &windowSizeInput, &pointNumberInput};

		while (initialConfig.isOpen())
		{

			sf::Event evnt;
			initialConfig.pollEvent(evnt);
			switch (evnt.type)
			{
			case sf::Event::Closed:
				exitToken = true;
				initialConfig.close();
				break;
			case sf::Event::KeyPressed:

				//std::cout << evnt.text.unicode << std::endl;
					
				if (evnt.text.unicode >= 26 && evnt.text.unicode <= 35)	//regular number keys
				{
					char tmp = (char)('0'+evnt.text.unicode-26);
						
					std::string add(1, tmp);

					for (int i = 0; i < textInputs.size(); i++)
						if (textInputs[i]->active == true)
							textInputs[i]->addText(add);
				}

				if (evnt.text.unicode >= 75 && evnt.text.unicode <= 84)	//numpad
				{
					char tmp = (char)('0' + evnt.text.unicode - 75);


					std::string add(1, tmp);

					for (int i = 0; i < textInputs.size(); i++)
						if (textInputs[i]->active == true)
							textInputs[i]->addText(add);
				}

				if (evnt.text.unicode == 36) //esc key to leave the config menu without starting the generator
				{
					exitToken = true;
					initialConfig.close();
				}

				if (evnt.text.unicode == 58) //enter to leave the config menu and start the generator with the inputted variables
				{
					initialConfig.close();
				}
					
				if (evnt.text.unicode == 59) //backspace - works better than ::Keyboard::isKeyPressed
				{
					for (int i = 0; i < textInputs.size(); i++)
						if (textInputs[i]->active == true && textInputs[i]->isEmptyString() == false)
							textInputs[i]->removeLastLetter();
				}

				if (evnt.text.unicode == 60) //tab to loop through the text inputs
				{
					for (int i = 0; i < textInputs.size(); i++)
						if (textInputs[i]->active == true)
						{
							textInputs[i]->active = false;
							textInputs[(i + 1) % textInputs.size()]->active = true;
							break;
						}
				}

				break;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for (int i = 0; i < textInputs.size(); i++)
					textInputs[i]->active = false;

				if (exit.isPressed(initialConfig))
				{
					exitToken = true;
					initialConfig.close();
				}
				if (start.isPressed(initialConfig))
				{
					initialConfig.close();
				}
				for (int i = 0; i < textInputs.size(); i++)
					textInputs[i]->isPresed(initialConfig);
			}

			initialConfig.draw(exit);
			initialConfig.draw(start);

			initialConfig.draw(canvasSizeInput);
			initialConfig.draw(windowSizeInput);
			initialConfig.draw(pointNumberInput);

			initialConfig.display();
		}
		
		if (exitToken)
			break;

		if (!canvasSizeInput.isEmptyString())
			canvasSize = canvasSizeInput.getValue();
		if (!windowSizeInput.isEmptyString())
			windowSize = windowSizeInput.getValue();
		if (!pointNumberInput.isEmptyString())
			nrPoints = pointNumberInput.getValue().x;

		sf::RectangleShape background(canvasSize);
		background.setFillColor(sf::Color(31, 31, 31, 255));

		/*
		do {
			std::cin >> nrPoints;//temporary
		} while (nrPoints < 3);
		exitToken = true;//also temporary
		*/


		sf::RenderWindow window(sf::VideoMode(windowSize.x,windowSize.y), "Probabilistic Fractal", sf::Style::Default);
		sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
		sf::RenderTexture tex;

		//window.setFramerateLimit(30);


		pixel.setFillColor(sf::Color::White);

		tex.create(canvasSize.x, canvasSize.y);
		tex.draw(background);

		points = new sf::Vector2f[nrPoints];

		for (int i = 0; i < nrPoints; i++)
		{
			points[i] = sf::Vector2f(canvasSize.x/2 + (canvasSize.x/2) * sin( 2.0*pi*i / nrPoints ), canvasSize.y/2 + (canvasSize.y/2) * -cos((2*pi*i) /nrPoints));
		}

		p = points[0];

		for (int i = 0; i < nrPoints; i++)
		{
			pixel.setPosition(points[i]);
			tex.draw(pixel);
		}

		tex.display();

		sf::Sprite sprite(tex.getTexture());
		fitSpriteToWindow(sprite, window);

		while (window.isOpen())
		{
			sf::Event evnt;
			window.pollEvent(evnt);
			if (evnt.type == sf::Event::Closed)
			{
				std::string filename("outputs/Probabilistic Fractal ");
				filename += std::to_string(nrPoints);
				filename += " Sides ";
				filename += std::to_string((int)canvasSize.x);
				filename += "x";
				filename += std::to_string((int)canvasSize.y);
				filename += ".png";
				tex.getTexture().copyToImage().saveToFile(filename);
				window.close();
			}

			int k = rand() % nrPoints;
			p += points[k] * float(nrPoints-2.0f);
			p /= (float)nrPoints-1;

			pixel.setPosition(p);


			tex.draw(pixel);
			tex.display();
			window.draw(sprite);
			window.display();
		}
	}

	return 0;
}