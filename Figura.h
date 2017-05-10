#pragma once
#include <SFML/Graphics.hpp>
#include <string.h>

enum RodzajFigury{
	b_pionek,
	b_wieza,
	b_skoczek,
	b_goniec,
	b_hetman,
	b_krol,
	c_pionek,
	c_wieza,
	c_skoczek,
	c_goniec,
	c_hetman,
	c_krol
};

class Figura : public sf::Drawable
{
public:
	Figura();
	Figura(const RodzajFigury& _rodzaj, const sf::Vector2f& _pozycja);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void setPosition(const sf::Vector2f& _pozycja);
	void wczytajSprite();
	bool czyWSrodku(const sf::Vector2f& _pozycja);
	bool wybor;
private:
	sf::Sprite sprite;
	sf::Texture texture;
	RodzajFigury rodzaj;
	sf::Vector2f pozycja;
	bool stan;
};