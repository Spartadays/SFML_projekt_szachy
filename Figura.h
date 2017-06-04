#pragma once
#include <SFML/Graphics.hpp>
#include <string.h>

enum RodzajFigury{
	pionek,
	wieza,
	skoczek,
	goniec,
	hetman,
	krol,
	nic
};

enum Kolor{
	bialy,
	czarny
};

class Figura : public sf::Drawable
{
public:
	Figura(const RodzajFigury& _rodzaj, const Kolor& _kolor, const sf::Vector2f& _pozycja);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPosition(const sf::Vector2f& _pozycja);
	void wczytajSprite();
	bool czyWSrodku(const sf::Vector2f& _pozycja);

	void zbij();

	bool wybor;
	bool stan;
	int x; //pozycja na planszy (od gornego lewego rogu)
	int y; //pozycja na planszy
	RodzajFigury rodzaj;
	Kolor kolor;
	int licznik_poruszania;
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2f pozycja;
};