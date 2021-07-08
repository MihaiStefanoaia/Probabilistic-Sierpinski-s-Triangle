#pragma once
#include <SFML/Graphics.hpp>
#include "button.h"

class textInput : public sf::Drawable
{
private:
	sf::Vector2f topLeft, bottomRight;
	sf::Vector2f inputTopLeft, inputBottomRight;
	sf::RenderTexture texture;
	sf::Sprite sprt;
	sf::Text inputText;
	sf::Text description;
	sf::Font fnt;
public:
	bool active;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateSprite();

public:
	textInput(sf::Vector2f position, sf::Vector2f size, sf::Vector2f inputPos, sf::Vector2f inputSize, std::string descriptionText, const char alignment[]="LC", bool debugMode = 0);
	void addText(std::string input);
	void removeLastLetter();
	bool isEmptyString();
	sf::Vector2f getValue();
	void isPresed(sf::RenderWindow& window);


};