#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
public:
	Button(std::string txt, sf::Vector2f size) {
		text.setString(txt);
		text.setFillColor(sf::Color::Black);
		text.setCharacterSize(16);

		button.setSize(size);
		button.setFillColor(sf::Color::White);
		button.setOutlineColor(sf::Color::Black);
		button.setOutlineThickness(1.0f);
	}

	void setFont(sf::Font& font) {
		text.setFont(font);
	}

	void setPosition(sf::Vector2f pos) {
		button.setPosition(pos);

		float xPos = (pos.x + button.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
		float yPos = (pos.y + button.getLocalBounds().height / 2) - (text.getLocalBounds().height / 2);
		text.setPosition({ xPos, yPos });
	}

	void drawTo(sf::RenderWindow& window) {
		window.draw(button);
		window.draw(text);
	}

	bool isMouseOver(sf::RenderWindow& window) {
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnX = button.getPosition().x;
		float btnY = button.getPosition().y;

		if (mouseX > btnX and mouseX < btnX + button.getLocalBounds().width and mouseY > btnY and mouseY < btnY + button.getLocalBounds().height)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	sf::RectangleShape button;
	sf::Text text;
};