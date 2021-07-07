#include "button.h"

button::button(int x1, int y1, int x2, int y2, std::string text)
{
	topLeft.x = x1;
	topLeft.y = y1;
	bottomRight.x = x2;
	bottomRight.y = y2;

	sf::String a(text);
	sf::Font fnt;
	fnt.loadFromFile("ARI.ttf");
	sf::Text tmp(a,fnt);
	this->text = tmp;

	sf::RectangleShape rim(sf::Vector2f(abs(topLeft.x - bottomRight.x), abs(topLeft.y - bottomRight.y)));

	rim.setFillColor(sf::Color(31, 31, 31, 255));
	rim.setOutlineThickness(2.0f);
	rim.setOutlineColor(sf::Color(255, 255, 255, 255));


	float width = tmp.getGlobalBounds().width;
	float height = tmp.getGlobalBounds().height;

	this->text.setOrigin(width / 2, height / 2);//centers the text onto the button
	this->text.setPosition(abs(topLeft.x + bottomRight.x) / 2, abs(topLeft.y + bottomRight.y) / 2);

	this->texture.create(abs(topLeft.x - bottomRight.x), abs(topLeft.y - bottomRight.y));
	this->texture.draw(rim);
	this->texture.draw(tmp);

	this->texture.display();
}

bool button::isPressed(sf::RenderWindow &window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2i tmp = sf::Mouse::getPosition(window);
		if (tmp.x >= topLeft.x && tmp.y >= topLeft.y && tmp.x <= bottomRight.x && tmp.y <= bottomRight.y)
			return true;
	}
	return false;
}

sf::RenderTexture& button::getTexture()
{
	return this->texture;
}