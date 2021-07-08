#pragma once
#include <SFML/Graphics.hpp>
#include <string>


class button : public sf::Drawable
{
	sf::Vector2f topLeft, bottomRight;
	sf::RenderTexture texture;
	sf::Sprite sprt;
	sf::Text txt;
	char alignment[3];
public:
	button(sf::Vector2f position, sf::Vector2f size, std::string text, const char alignment[], bool debugMode = 0);
	bool isPressed(sf::RenderWindow& window);
	void setString(sf::String str);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateSprite(bool debugMode = 0);
};
