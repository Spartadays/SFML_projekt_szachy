#include "Figura.h"
#include <iostream>

Figura::Figura(const RodzajFigury& _rodzaj, const Kolor& _kolor, const sf::Vector2f& _pozycja)
{
	rodzaj = _rodzaj;
	pozycja = _pozycja;
	kolor = _kolor;
	stan = true;
	wybor = false;
	licznik_poruszania = 0;
	if (_kolor == bialy)
	{
		switch (_rodzaj)
		{
		case pionek:
			texture.loadFromFile("b_pionek.png");
			break;
		case wieza:
			texture.loadFromFile("b_wieza.png");
			break;
		case skoczek:
			texture.loadFromFile("b_skoczek.png");
			break;
		case goniec:
			texture.loadFromFile("b_goniec.png");
			break;
		case hetman:
			texture.loadFromFile("b_hetman.png");
			break;
		case krol:
			texture.loadFromFile("b_krol.png");
			break;
		}
	}
	else if (_kolor == czarny)
	{
		switch (_rodzaj)
		{
		case pionek:
			texture.loadFromFile("c_pionek.png");
			break;
		case wieza:
			texture.loadFromFile("c_wieza.png");
			break;
		case skoczek:
			texture.loadFromFile("c_skoczek.png");
			break;
		case goniec:
			texture.loadFromFile("c_goniec.png");
			break;
		case hetman:
			texture.loadFromFile("c_hetman.png");
			break;
		case krol:
			texture.loadFromFile("c_krol.png");
			break;
		}
	}
	sprite.setTexture(texture);
	sprite.setOrigin(30, 30);
	sprite.setPosition(pozycja);
}

void Figura::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//rysowanie obiektow, kt�re sk�adaj� si� na narysowanie naszej postaci
	if (stan) target.draw(sprite);
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

void Figura::zbij()
{
	stan = false;
	wybor = false;
	x = 1500;
	y = 1500;
	this->setPosition(sf::Vector2f(1500, 1500));
}

bool Figura::logika(int _x, int _y, bool zajete)
{
	switch (rodzaj)
	{
	case pionek:
	{
		int pomocnicza = 1;
		if (kolor == czarny) pomocnicza = -1;
		if ((_y == y - 1 * pomocnicza || (_y == y - 2 * pomocnicza && licznik_poruszania == 0)) && (_x == x) && (!zajete))
		{
			return true;
		}
		else if (zajete && (_x == x + 1 || _x == x - 1) && _y == y - 1 * pomocnicza)
		{
			return true;
		}
		else
			return false;
	}
	default:
		return true;
	}
}