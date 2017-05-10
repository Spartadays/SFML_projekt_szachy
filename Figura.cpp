#include "Figura.h"

Figura::Figura()
{
	texture.loadFromFile("x.png");
	sprite.setTexture(texture);
	stan = true;
	wybor = false;
}
void Figura::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//rysowanie obiektow, które sk³adaj¹ siê na narysowanie naszej postaci
	target.draw(sprite);
}
Figura::Figura(const RodzajFigury& _rodzaj, const sf::Vector2f& _pozycja)
{
	rodzaj = _rodzaj;
	pozycja = _pozycja;
	switch (_rodzaj)
	{
	case b_pionek:
		texture.loadFromFile("b_pionek.png");
		break;
	case b_wieza:
		texture.loadFromFile("b_wieza.png");
		break;
	case b_skoczek:
		texture.loadFromFile("b_skoczek.png");
		break;
	case b_goniec:
		texture.loadFromFile("b_goniec.png");
		break;
	case b_hetman:
		texture.loadFromFile("b_hetman.png");
		break;
	case b_krol:
		texture.loadFromFile("b_krol.png");
		break;
	case c_pionek:
		texture.loadFromFile("c_pionek.png");
		break;
	case c_wieza:
		texture.loadFromFile("c_wieza.png");
		break;
	case c_skoczek:
		texture.loadFromFile("c_skoczek.png");
		break;
	case c_goniec:
		texture.loadFromFile("c_goniec.png");
		break;
	case c_hetman:
		texture.loadFromFile("c_hetman.png");
		break;
	case c_krol:
		texture.loadFromFile("c_krol.png");
		break;
	}
	sprite.setTexture(texture);
	sprite.setOrigin(30, 30);
	sprite.setPosition(pozycja);
}
void Figura::setPosition(const sf::Vector2f& _pozycja)
{
	sprite.setPosition(_pozycja);
	pozycja = _pozycja;
}
void Figura::wczytajSprite()
{
	sprite.setTexture(texture);
}
bool Figura::czyWSrodku(const sf::Vector2f& _pozycja)
{
	if (pozycja.x - 50 <= _pozycja.x && pozycja.x + 50 >= _pozycja.x && pozycja.y - 50 <= _pozycja.y && pozycja.y + 50 >= _pozycja.y)
	{
		return true;
	}
	else
		return false;
}