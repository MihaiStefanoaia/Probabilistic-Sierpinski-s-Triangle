#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class button
{
	sf::Vector2f topLeft, bottomRight;
	sf::RenderTexture texture;
	sf::Text text;
public:
	button(int x1, int y1, int x2, int y2, std::string text);
	bool isPressed(sf::RenderWindow& window);
	sf::RenderTexture& getTexture();
};
