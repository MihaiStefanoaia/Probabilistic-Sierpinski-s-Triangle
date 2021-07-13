#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "button.h"
#include "textInput.h"
#include <array>
#include <thread>

const float pi = 3.14159f;


void fitSpriteToWindow(sf::Sprite& sprite, sf::Window& window) //sets scale of the spirte to fit perfectly inside the screen
{
	sf::Vector2u windowSize = window.getSize(), spriteSize = sprite.getTexture()->getSize();
	sf::Vector2f scl((float)windowSize.x, (float)windowSize.y);
	scl.x /= spriteSize.x;
	scl.y /= spriteSize.y;
	sprite.setScale(scl);
}

void generateTextureData(sf::Window& window, bool** matrix, sf::Vector2f *points, int nrPoints, bool &pause)
{
	sf::Vector2f p = points[0];
	int k;
	while (window.isOpen())
	{
		while(!pause)
		{
			k = rand() % nrPoints;
			p += points[k] * float(nrPoints - 2.0f);
			p /= (float)nrPoints - 1;

			matrix[(int)p.x][(int)p.y] = true;
		}
	}

}

void updateTexture(sf::RenderTexture& tex, bool** matrix, sf::Vector2f canvasSize)
{
	sf::RectangleShape pixel(sf::Vector2f(1.0f,1.0f));
	pixel.setFillColor(sf::Color::White);

	for (int i = 0; i < canvasSize.x; i++)
	{
		for (int j = 0; j < canvasSize.y; j++)
		{
			if (matrix[i][j] == true)
			{
				pixel.setPosition(sf::Vector2f(i, j));
				tex.draw(pixel);
			}
		}
	}
	
	tex.display();
}

int main()
{
	bool exitToken = 0;
	int nrPoints=3;

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




		sf::RenderWindow window(sf::VideoMode(windowSize.x,windowSize.y), "Probabilistic Fractal", sf::Style::Default);
		window.setFramerateLimit(30);
		
		bool** matrix;
		matrix = new bool*[canvasSize.x];
		for (int i = 0; i < canvasSize.x; i++)
		{
			matrix[i] = new bool[canvasSize.y]{false};
		}

		bool** matrixCopy;
		matrixCopy = new bool* [canvasSize.x];
		for (int i = 0; i < canvasSize.x; i++)
		{
			matrixCopy[i] = new bool[canvasSize.y]{false};
		}

		sf::RectangleShape background(canvasSize);
		background.setFillColor(sf::Color(31, 31, 31, 255));

		sf::RenderTexture tex;
		tex.create(canvasSize.x, canvasSize.y);
		tex.draw(background);
		tex.display();
		sf::Sprite sprite(tex.getTexture());
		fitSpriteToWindow(sprite, window);
		
		sf::Vector2f p, * points;
		points = new sf::Vector2f[nrPoints];

		for (int i = 0; i < nrPoints; i++)
			points[i] = sf::Vector2f(canvasSize.x/2 + (canvasSize.x/2) * sin( 2.0*pi*i / nrPoints ), canvasSize.y/2 + (canvasSize.y/2) * -cos((2*pi*i) /nrPoints));
		p = points[0];

		bool ThreadLock = 0;
		std::thread generate(generateTextureData, std::ref(window), std::ref(matrix), std::ref(points), nrPoints, std::ref(ThreadLock));
		generate.detach();

		while (window.isOpen())
		{
			sf::Event evnt;
			window.pollEvent(evnt);
			if (evnt.type == sf::Event::Closed)
			{
				ThreadLock = true;
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

			ThreadLock = true;

			for (int i = 0; i < canvasSize.x; i++)
			{
				for (int j = 0; j < canvasSize.y; j++)
					matrixCopy[i][j] = matrix[i][j];
			}

			ThreadLock = false;

			updateTexture(tex, matrixCopy, canvasSize);

			window.draw(sprite);
			window.display();
		}
	}

	return 0;
}