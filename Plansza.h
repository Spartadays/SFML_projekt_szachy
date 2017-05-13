#pragma once
#include <SFML/Graphics.hpp>

class Plansza : sf::RectangleShape
{
public:
	Plansza(int rozmiar_pola);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual sf::Vector2f getPosition(int x, int y);
private:
	int rozmiar_pola;
};
