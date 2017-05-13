#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Figura.h"
#include "Plansza.h"
#include <string.h>

/*Wymiary okna:*/
#define wysokosc_okna 1000
#define szerokosc_okna 1600
#define rozmiar_pola 110
/*_____________*/

sf::ContextSettings settings;
sf::RenderWindow window(sf::VideoMode(szerokosc_okna, wysokosc_okna), "My window", sf::Style::Close, settings);

enum Kolor{
	bialy,
	czarny
};

struct Siatka
{
	bool zajete;
	Kolor kolor;
};

/*OBIEKTY DO GRY:*/
std::vector <Figura> figury;
Plansza pola(rozmiar_pola);
std::vector <std::vector<Siatka>>mapa;
/*_______________*/

void ustawieniaOkna()
{
	//window.setVerticalSyncEnabled(true); //vsync
	window.setFramerateLimit(60); // argument: iloœæ fps (nigdy nie u¿ywaj obu na raz)
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
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f pos_myszki = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			for (auto itr = figury.begin(); itr != figury.end(); itr++)
			{
				if ((*itr).czyWSrodku(pos_myszki))
				{
					std::cout << "XX" << std::endl;
					(*itr).wybor = true;
				}
				else
					(*itr).wybor = false;
			}
		}
		if (event.type == sf::Event::MouseMoved)
		{
			sf::Vector2f pos_myszki = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			for (auto itr = figury.begin(); itr != figury.end(); itr++)
			{
				if ((*itr).wybor == true)
				{
					(*itr).setPosition(pos_myszki);
				}
			}
		}
		if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f pos_myszki = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			for (auto itr = figury.begin(); itr != figury.end(); itr++)
			{
				if ((*itr).wybor == true)
				{
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 8; j++)
						{
							if ((*itr).czyWSrodku(pola.getPosition(i, j)))
							{
								(*itr).setPosition(pola.getPosition(i, j));
							}
						}
					}

					(*itr).wybor = false;
				}
			}
		}
	}
}

void rysowanie()
{
	window.clear(sf::Color::Black);		// czyszczenie okna na czarny kolor (nie trzeba podawaæ argumentu, standardowo czyœci siê na czarny)
	pola.draw(window, sf::RenderStates::Default);
	for (auto itr = figury.begin(); itr != figury.end(); itr++)
	{
		//window.draw((*itr));
		(*itr).draw(window, sf::RenderStates::Default);
		(*itr).wczytajSprite();
	}
	window.display();					// koniec tej klatki
}

void tworzenieFigur()
{
	for (int i = 0; i < 8; i++)
	{
		Figura bialy_pion(b_pionek, pola.getPosition(i, 1));
		figury.push_back(bialy_pion);
		mapa[i][1].kolor = bialy;
		mapa[i][1].zajete = true;

		Figura czarny_pion(c_pionek, pola.getPosition(i, 6));
		figury.push_back(czarny_pion);
		mapa[i][6].kolor = czarny;
		mapa[i][6].zajete = true;
	}

	for (int i = 0; i < 2; i++)
	{
		Figura biala_wieza(b_wieza, pola.getPosition(0 + i * 7, 0));
		Figura czarna_wieza(c_wieza, pola.getPosition(0 + i * 7, 7));
		figury.push_back(biala_wieza);
		mapa[0 + i * 7][0].kolor = bialy;
		mapa[0 + i * 7][0].zajete = true;
		figury.push_back(czarna_wieza);
		mapa[0 + i * 7][7].kolor = czarny;
		mapa[0 + i * 7][7].zajete = true;

		Figura bialy_skoczek(b_skoczek, pola.getPosition(1 + i * 5, 0));
		Figura czarny_skoczek(c_skoczek, pola.getPosition(1 + i * 5, 7));
		figury.push_back(bialy_skoczek);
		mapa[1 + i * 5][0].kolor = bialy;
		mapa[1 + i * 5][0].zajete = true;
		figury.push_back(czarny_skoczek);
		mapa[1 + i * 5][7].kolor = czarny;
		mapa[1 + i * 5][7].zajete = true;

		Figura bialy_goniec(b_goniec, pola.getPosition(2 + i * 3, 0));
		Figura czarny_goniec(c_goniec, pola.getPosition(2 + i * 3, 7));
		figury.push_back(bialy_goniec);
		mapa[2 + i * 3][0].kolor = bialy;
		mapa[2 + i * 3][0].zajete = true;
		figury.push_back(czarny_goniec);
		mapa[2 + i * 3][7].kolor = czarny;
		mapa[2 + i * 3][7].zajete = true;
	}

	Figura czarny_hetman(c_hetman, pola.getPosition(4, 7));
	figury.push_back(czarny_hetman);
	mapa[4][7].kolor = czarny;
	mapa[4][7].zajete = true;
	Figura czarny_krol(c_krol, pola.getPosition(3, 7));
	figury.push_back(czarny_krol);
	mapa[3][7].kolor = czarny;
	mapa[3][7].zajete = true;

	Figura bialy_hetman(b_hetman, pola.getPosition(4, 0));
	figury.push_back(bialy_hetman);
	mapa[4][0].kolor = bialy;
	mapa[4][0].zajete = true;
	Figura bialy_krol(b_krol, pola.getPosition(3, 0));
	figury.push_back(bialy_krol);
	mapa[3][0].kolor = bialy;
	mapa[3][0].zajete = true;
}

int main()
{
	ustawieniaOkna();

	for (int i = 0; i < 8; i++)
	{
		std::vector<Siatka> poz;
		for (int j = 0; j < 8; j++)
		{
			Siatka siatka;
			poz.push_back(siatka);
		}
		mapa.push_back(poz);
	}
	tworzenieFigur();
	while (window.isOpen())						// program bêdzie w³¹czony dopóki okno jest otwarte
	{
		obslugaKlawiaturyIMyszy();
		rysowanie();
	}

	return 0;
}