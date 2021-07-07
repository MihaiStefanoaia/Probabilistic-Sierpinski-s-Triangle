#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "button.h"

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

int main()
{
	bool exitToken = 0;
	int nrPoints=3;
	sf::Vector2f sizes(takeFromFile());
	sf::Vector2f p, * points;

	sf::RectangleShape background(sf::Vector2f(sizes.x, sizes.y));
	background.setFillColor(sf::Color(31,31,31,255));
	

	while (!exitToken)
	{
		//needs to be modified a ~~bit~~ lot
		
		sf::RenderWindow initialConfig(sf::VideoMode(500, 300), "Configuration", sf::Style::Default);
		button exit(25,200,225,300,"Exit"), start(275, 200, 475, 300,"Start");

		/*
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
			case sf::Event::MouseButtonPressed:
				if (exit.isPressed(initialConfig))
				{
					exitToken = true;
					initialConfig.close();
				}
				if (start.isPressed(initialConfig))
				{
					initialConfig.close();
				}
				break;

			}

			initialConfig.draw(sf::Sprite(exit.getTexture().getTexture()));

		}
		*/
		
		do {
			std::cin >> nrPoints;//temporary
		} while (nrPoints < 3);
		exitToken = true;//also temporary

		sf::RenderWindow window(sf::VideoMode(1000, 1000), "Probabilistic Fractal", sf::Style::Default);
		sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
		sf::RenderTexture tex;


		pixel.setFillColor(sf::Color::White);

		tex.create(sizes.x, sizes.y);
		tex.draw(background);

		points = new sf::Vector2f[nrPoints];

		for (int i = 0; i < nrPoints; i++)
		{
			points[i] = sf::Vector2f(sizes.x/2 + (sizes.x/2) * sin( 2.0*pi*i / nrPoints ), sizes.y/2 + (sizes.y/2) * -cos((2*pi*i) /nrPoints));
		}

		p = points[0];

		for (int i = 0; i < nrPoints; i++)
		{
			pixel.setPosition(points[i]);
			tex.draw(pixel);
		}

		tex.display();

		sf::Sprite sprite(tex.getTexture());
		sf::Vector2u scaleTemp = window.getSize();
		sf::Vector2f scl((float)scaleTemp.x, (float)scaleTemp.y);
		scl.x /= sizes.x;
		scl.y /= sizes.y;
		sprite.setScale(scl);

		while (window.isOpen())
		{
			sf::Event evnt;
			window.pollEvent(evnt);
			if (evnt.type == sf::Event::Closed)
			{
				std::string filename("outputs/Probabilistic Fractal ");
				filename += std::to_string(nrPoints);
				filename += " Sides.png";
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