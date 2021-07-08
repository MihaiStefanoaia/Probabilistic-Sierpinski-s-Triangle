#include "button.h"

button::button(sf::Vector2f position, sf::Vector2f size, std::string text, const char alignment[], bool debugMode)
{
	this->topLeft = position;
	this->bottomRight = position + size;
	this->alignment[0] = alignment[0];
	this->alignment[1] = alignment[1];
	this->alignment[2] = NULL;

	this->texture.create(size.x, size.y);

	sf::String a(text);
	sf::Font fnt;
	fnt.loadFromFile("ARI.ttf");
	sf::Text tmp(a,fnt);

	this->txt = tmp;

	updateSprite(debugMode);

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

void button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprt);
}

void button::setString(sf::String str)
{
	this->txt.setString(str);
	updateSprite();
}

void button::updateSprite(bool debugMode)
{
	sf::Vector2f size(bottomRight - topLeft);
	sf::RectangleShape rim(size);

	rim.setFillColor(sf::Color(31, 31, 31, 255));
	rim.setOutlineColor(sf::Color(255, 255, 255, 255));
	rim.setOutlineThickness(-1.0f);

	sf::FloatRect rect;
	rect = txt.getGlobalBounds();

	sf::Vector2f originAlign(0.0f, 0.0f), positionAlign(0.0f, 0.0f);

	switch (alignment[0])	//Horizontal alignment
	{
	case 'L':	//to the left
		originAlign.x = 0.0f;
		positionAlign.x = 2.0f + abs(rim.getOutlineThickness());
		break;
	case 'C':	//centered
		originAlign.x = rect.left + rect.width / 2;
		positionAlign.x = size.x / 2.0f;
		break;
	case 'R':	//to the right
		originAlign.x = rect.width;
		positionAlign.x = size.x - 2.0f - abs(rim.getOutlineThickness());
		break;
	}

	switch (alignment[1])	//Vertical alignment
	{
	case 'T':	//to the top
		originAlign.y = 0.0f;
		positionAlign.y = 2.0f + abs(rim.getOutlineThickness());
		break;
	case 'C':	//centered
		originAlign.y = rect.top + rect.height / 2;
		positionAlign.y = size.y / 2.0f;
		break;
	case 'B':	//to the bottom
		originAlign.y = rect.height;
		positionAlign.y = size.y - 2.0f - abs(rim.getOutlineThickness());
		break;
	}

	txt.setOrigin(originAlign);
	txt.setPosition(positionAlign);





	this->texture.draw(rim);
	this->texture.draw(txt);

	if (debugMode)
	{
		sf::RectangleShape debug(sf::Vector2f(rect.width, rect.height));
		debug.setOutlineColor(sf::Color(255, 255, 255, 255));
		debug.setFillColor(sf::Color(0, 0, 0, 0));
		debug.setOutlineThickness(-1.0f);
		debug.setOrigin(originAlign);
		debug.setPosition(positionAlign);
		this->texture.draw(debug);
	}

	this->texture.display();

	this->sprt.setPosition(topLeft);
	this->sprt.setTexture(texture.getTexture());

}