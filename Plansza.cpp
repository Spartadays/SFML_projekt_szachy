#include "Plansza.h"

std::vector <std::vector<sf::RectangleShape>>pola;

Plansza::Plansza(int _rp)
{
	rozmiar_pola = _rp;

	for (int i = 0; i < 8; i++)
	{
		std::vector<sf::RectangleShape> poziomy;
		for (int j = 0; j < 8; j++)
		{
			sf::RectangleShape Plansza(sf::Vector2f(rozmiar_pola, rozmiar_pola));
			poziomy.push_back(Plansza);
		}
		pola.push_back(poziomy);
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			pola[i][j].setFillColor(sf::Color::Blue);
			if ((i % 2 == 0) && (j % 2 == 0))
			{
				pola[i][j].setFillColor(sf::Color::White);
			}
			else if ((i % 2 != 0) && (j % 2 != 0))
			{
				pola[i][j].setFillColor(sf::Color::White);
			}
			pola[i][j].setOutlineColor(sf::Color::Black);
			pola[i][j].setOutlineThickness(3);
			pola[i][j].setOrigin(rozmiar_pola / 2.0, rozmiar_pola / 2.0);
			pola[i][j].setPosition(80 + rozmiar_pola*i, 80 + rozmiar_pola*j);
		}
	}
}

void Plansza::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			target.draw(pola[i][j]);
			//pola[i][j].rotate(1);
		}
	}
}

sf::Vector2f Plansza::getPosition(int x, int y)
{
	return pola[x][y].getPosition();
}