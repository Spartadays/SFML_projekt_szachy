#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Figura.h"
#include <string.h>

/*Wymiary okna:*/
#define wysokosc_okna 1000
#define szerokosc_okna 1600
#define rozmiar_pola 110
/*_____________*/

sf::ContextSettings settings;
sf::RenderWindow window(sf::VideoMode(szerokosc_okna, wysokosc_okna), "My window", sf::Style::Close, settings);

/*OBIEKTY DO GRY:*/
std::vector <std::vector<sf::RectangleShape>>pola;
std::vector <Figura> figury;

/*_______________*/

void ustawieniaOkna()
{
	//window.setVerticalSyncEnabled(true); //vsync
	window.setFramerateLimit(60); // argument: iloœæ fps (nigdy nie u¿ywaj obu na raz)
}

void tworzeniePlanszy()
{
	for (int i = 0; i < 8; i++)
	{
		std::vector<sf::RectangleShape> poziomy;
		for (int j = 0; j < 8; j++)
		{
			sf::RectangleShape pole(sf::Vector2f(rozmiar_pola, rozmiar_pola));
			poziomy.push_back(pole);
		}
		pola.push_back(poziomy);
	}
}

void obslugaKlawiaturyIMyszy()
{
	sf::Event event;

	while (window.pollEvent(event))			// dopóki s¹ eventy do obs³u¿enia
	{
		switch (event.type)					// sprawdzenie typu eventów
		{
		case sf::Event::Closed:				// zamkniêcie okna
			window.close();
			break;
		case sf::Event::KeyPressed:			// wciœniêcie klawisza (dowolnego)
			break;
		case sf::Event::Resized:			// gdy uzytkownik zmieni rozmiar okna
		{
			std::cout << "nowa szerokosc: " << event.size.width << std::endl;
			std::cout << "nowa wysokosc: " << event.size.height << std::endl;
		}
		default:
			break;
		}
	}
}

void rysowanie()
{
	window.clear(sf::Color::Black);		// czyszczenie okna na czarny kolor (nie trzeba podawaæ argumentu, standardowo czyœci siê na czarny)
	// rysowanie
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			window.draw(pola[i][j]);
		}
	}
	for (auto itr = figury.begin(); itr != figury.end(); itr++)
	{
		//window.draw((*itr));
		(*itr).draw(window, sf::RenderStates::Default);
		(*itr).wczytajSprite();
	}
	window.display();					// koniec tej klatki
}

void planszaKolor()
{
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

int main()
{
	tworzeniePlanszy();
	ustawieniaOkna();
	planszaKolor();

	for (int i = 0; i < 8; i++)
	{
		Figura bialy_pion(b_pionek, pola[i][1].getPosition());
		figury.push_back(bialy_pion);

		Figura czarny_pion(c_pionek, pola[i][6].getPosition());
		figury.push_back(czarny_pion);
	}

	for (int i = 0; i < 2; i++)
	{
		Figura biala_wieza(b_wieza, pola[0 + i * 7][0].getPosition());
		Figura czarna_wieza(c_wieza, pola[0 + i * 7][7].getPosition());
		figury.push_back(biala_wieza);
		figury.push_back(czarna_wieza);

		Figura bialy_skoczek(b_skoczek, pola[1 + i * 5][0].getPosition());
		Figura czarny_skoczek(c_skoczek, pola[1 + i * 5][7].getPosition());
		figury.push_back(bialy_skoczek);
		figury.push_back(czarny_skoczek);

		Figura bialy_goniec(b_goniec, pola[2 + i * 3][0].getPosition());
		Figura czarny_goniec(c_goniec, pola[2 + i * 3][7].getPosition());
		figury.push_back(bialy_goniec);
		figury.push_back(czarny_goniec);
	}

	Figura czarny_hetman(c_hetman, pola[4][7].getPosition());
	figury.push_back(czarny_hetman);
	Figura czarny_krol(c_krol, pola[3][7].getPosition());
	figury.push_back(czarny_krol);

	Figura bialy_hetman(b_hetman, pola[4][0].getPosition());
	figury.push_back(bialy_hetman);
	Figura bialy_krol(b_krol, pola[3][0].getPosition());
	figury.push_back(bialy_krol);

	while (window.isOpen())						// program bêdzie w³¹czony dopóki okno jest otwarte
	{
		obslugaKlawiaturyIMyszy();
		rysowanie();
	}

	return 0;
}