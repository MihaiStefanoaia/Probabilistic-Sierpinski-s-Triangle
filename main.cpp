#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "button.h"
#include "textInput.h"

const float pi = 3.14159;
sf::Vector2f takeFromFile()
{
	std::ifstream cfg;
	cfg.open("config.txt");
	float x, y;
	cfg >> x >> y;
	return { x,y };
	cfg.close();
}


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
	//sf::Vector2f canvas(takeFromFile());
	sf::Vector2f p, * points;

	
	
	

	while (!exitToken)
	{
		//needs to be modified a ~~bit~~ lot
		char checkErr;
		sf::Vector2f canvas(3000,3000);
		sf::Vector2f windowSiz(1000,1000);


		sf::RenderWindow initialConfig(sf::VideoMode(500, 300), "Configuration", sf::Style::Default);
		button exit(sf::Vector2f(25, 200), sf::Vector2f(200, 100), "Exit", "CC"), start(sf::Vector2f(275, 200), sf::Vector2f(200, 100), "Start", "CC");
		textInput canvasSize(sf::Vector2f(0, 0), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Canvas Size (px):");
		textInput windowSize(sf::Vector2f(0, 50), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Window Size (px):");
		textInput pointNumber(sf::Vector2f(0, 100), sf::Vector2f(500, 50), sf::Vector2f(300, 0), sf::Vector2f(200, 50), "Number of Points:");

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
					
				if (evnt.text.unicode >= 26 && evnt.text.unicode <= 35)
				{
					char tmp = (char)('0'+evnt.text.unicode-26);
					
						
					std::string add(1, tmp);
					if (canvasSize.active == true)
						canvasSize.addText(add);
					if (windowSize.active == true)
						windowSize.addText(add);
					if (pointNumber.active == true)
						pointNumber.addText(add);
					
				}
					
				if (sf::Keyboard::isKeyPressed( sf::Keyboard::BackSpace))
				{
					if (canvasSize.active == true && !canvasSize.isEmptyString())
						canvasSize.removeLastLetter();
					if (windowSize.active == true && !windowSize.isEmptyString())
						windowSize.removeLastLetter();
					if (pointNumber.active == true && !pointNumber.isEmptyString())
						pointNumber.removeLastLetter();
				}

				break;

			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				canvasSize.active = false;
				windowSize.active = false;
				pointNumber.active = false;

				if (exit.isPressed(initialConfig))
				{
					exitToken = true;
					initialConfig.close();
				}
				if (start.isPressed(initialConfig))
				{
					initialConfig.close();
				}
				canvasSize.isPresed(initialConfig);
				windowSize.isPresed(initialConfig);
				pointNumber.isPresed(initialConfig);
			}

			



			initialConfig.draw(exit);
			initialConfig.draw(start);
			initialConfig.draw(canvasSize);
			initialConfig.draw(windowSize);
			initialConfig.draw(pointNumber);
			initialConfig.display();
		}

		
		if (exitToken)
			break;
		
		canvas = canvasSize.getValue();
		windowSiz = windowSize.getValue();
		nrPoints = pointNumber.getValue().x;

		sf::RectangleShape background(canvas);
		background.setFillColor(sf::Color(31, 31, 31, 255));

		/*
		do {
			std::cin >> nrPoints;//temporary
		} while (nrPoints < 3);
		exitToken = true;//also temporary
		*/


		sf::RenderWindow window(sf::VideoMode(windowSiz.x,windowSiz.y), "Probabilistic Fractal", sf::Style::Default);
		sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
		sf::RenderTexture tex;

		


		pixel.setFillColor(sf::Color::White);

		tex.create(canvas.x, canvas.y);
		tex.draw(background);

		points = new sf::Vector2f[nrPoints];

		for (int i = 0; i < nrPoints; i++)
		{
			points[i] = sf::Vector2f(canvas.x/2 + (canvas.x/2) * sin( 2.0*pi*i / nrPoints ), canvas.y/2 + (canvas.y/2) * -cos((2*pi*i) /nrPoints));
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
				filename += std::to_string(canvas.x);
				filename += "x";
				filename += std::to_string(canvas.x);
				filename += ".png";
				tex.getTexture().copyToImage().saveToFile(filename);
				window.close();
			}

			int k = rand() % nrPoints;
			//for(int i=0;i<nrPoints-2;i++)
			p += points[k] * float(nrPoints-2.0f);
			p /= (float)nrPoints-1;

			pixel.setPosition(p);


			tex.draw(pixel);
			tex.display();
			window.draw(sprite);
			//window.draw(pixel);
			window.display();
			//window.draw(pixel);
		}
	}

	return 0;
}