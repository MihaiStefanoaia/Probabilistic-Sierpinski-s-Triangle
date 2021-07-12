#include "textInput.h"
#include <iostream>
#include <sstream>

textInput::textInput(sf::Vector2f position, sf::Vector2f size, sf::Vector2f inputPos, sf::Vector2f inputSize, std::string descriptionText, const char alignment[], bool debugMode)
{
	this->active = 0;
	this->topLeft = position;
	this->bottomRight = position + size;
	this->inputTopLeft = inputPos;
	this->inputBottomRight = inputPos + inputSize;

	
	this->fnt.loadFromFile("ARI.ttf");

	this->description.setString(descriptionText);
	this->description.setFont(fnt);
	
	this->inputText.setString("1");
	this->inputText.setFont(fnt);
	this->inputText.setFillColor(sf::Color(0, 0, 0, 255));

	this->texture.create(size.x, size.y);

	sf::FloatRect rect;
	rect = this->description.getGlobalBounds();

	sf::Vector2f originAlign(0.0f, rect.top + rect.height / 2), positionAlign(1.0f, size.y / 2);

	this->description.setOrigin(originAlign);
	this->description.setPosition(positionAlign);

	rect = this->inputText.getGlobalBounds();
	originAlign = sf::Vector2f(0.0f, rect.top + rect.height / 2);
	positionAlign = sf::Vector2f(inputTopLeft.x + 2.0f, (inputTopLeft.y + inputBottomRight.y) / 2 + 2.0f);

	this->inputText.setOrigin(originAlign);
	this->inputText.setPosition(positionAlign);

	this->removeLastLetter();

	this->updateSprite();

}

void textInput::addText(std::string input)
{
	sf::String temp = this->inputText.getString();
	temp += input;
	this->inputText.setString(  this->inputText.getString() + input);
	this->updateSprite();
}

void textInput::removeLastLetter()
{
	std::string temp = (std::string)this->inputText.getString();
	temp.pop_back();
	this->inputText.setString(temp);

	this->updateSprite();
}

void textInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprt);
}

void textInput::updateSprite()
{
	this->texture.clear();

	sf::Vector2f size(bottomRight - topLeft);
	sf::RectangleShape rim(size);

	rim.setFillColor(sf::Color(31, 31, 31, 255));
	rim.setOutlineColor(sf::Color(255, 255, 255, 255));
	rim.setOutlineThickness(-0.1f);




	
	texture.draw(rim);
	texture.draw(this->description);
	
	sf::Vector2f inputSize;
	inputSize = inputBottomRight - inputTopLeft;
	inputSize.x -= 4.0f;
	inputSize.y -= 4.0f;


	sf::RectangleShape textBox(inputSize);

	textBox.setFillColor(sf::Color::White);
	textBox.setPosition(inputTopLeft.x+2.0f,inputTopLeft.y+2.0f);

	texture.draw(textBox);


	
	

	texture.draw(this->inputText);
	
	texture.display();
	sprt.setPosition(topLeft);
	sprt.setTexture(texture.getTexture());

}

void textInput::isPresed(sf::RenderWindow& window)
{
	sf::Vector2i tmp = sf::Mouse::getPosition(window);
	if (tmp.x >= inputTopLeft.x + topLeft.x && tmp.y >= topLeft.y && tmp.x <= inputBottomRight.x + bottomRight.x && tmp.y <= bottomRight.y)
		this->active = true;
}

bool textInput::isEmptyString()
{
	return this->inputText.getString().getSize() == 0;
}

sf::Vector2f textInput::getValue()
{
	std::stringstream bruhs;
	float out;
	bruhs << (std::string)this->inputText.getString();
	bruhs >> out;
	std::cout << out<<"\n";
	return sf::Vector2f(out, out);
}

const sf::Vector2f textInput::getPosition()
{
	return this->topLeft;
}

const sf::Vector2f textInput::getSize()
{
	return this->bottomRight - this->topLeft;
}

const sf::Vector2f textInput::getInputPosition()
{
	return this->inputTopLeft;
}

const sf::Vector2f textInput::getInputSize()
{
	return this->inputBottomRight - this->inputTopLeft;
}