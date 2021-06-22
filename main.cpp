#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>

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
	sf::Vector2f sizes(takeFromFile()),div2(2,2),div3(3,3);
	sf::Vector2f p, p1(sizes.x/2,0), p2(0,sizes.y), p3(sizes);

	p = p1 + p2;
	p /= 2.0f;

	sf::RenderWindow window(sf::VideoMode(1000,1000), "Probabilistic Sierpinski's Triangle", sf::Style::Default);
	sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
	sf::RenderTexture tex;
	
	sf::RectangleShape background(sf::Vector2f(sizes.x, sizes.y));
	background.setFillColor(sf::Color::Black);
	pixel.setFillColor(sf::Color::White);
	
	tex.create(sizes.x, sizes.y);
	tex.draw(background);
	pixel.setPosition(p1);
	tex.draw(pixel);
	pixel.setPosition(p2);
	tex.draw(pixel);
	pixel.setPosition(p3);
	tex.draw(pixel);
	tex.display();

	sf::Sprite sprite(tex.getTexture());
	sf::Vector2u scaleTemp=window.getSize();
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
			tex.getTexture().copyToImage().saveToFile("Probabilistic Sierpinski's Triangle.png");
			window.close();
		}
		int k = rand()%3;

		switch (k)
		{
		case 0:
			p += p1;
			break;
		case 1:
			p += p2;
			break;
		case 2:
			p += p3;
			break;
		}
		p /= 2.0f;

		pixel.setPosition(p);


		tex.draw(pixel);
		tex.display();
		window.draw(sprite);
		//window.draw(pixel);
		window.display();
		//window.draw(pixel);
	}

	

	return 0;
}