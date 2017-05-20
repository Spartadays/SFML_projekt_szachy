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
sf::RenderWindow window(sf::VideoMode(szerokosc_okna, wysokosc_okna), "Szachy", sf::Style::Close, settings);

struct Siatka
{
	bool zajete = false;
	Kolor kolor;
	RodzajFigury rodzaj_figury;
};

/*_______________*/
std::vector <Figura> figury;
std::vector <std::vector<sf::RectangleShape>>pola;
std::vector <std::vector<Siatka>>mapa;
bool podniesiona = false;
sf::Font font;

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
		if (event.type == sf::Event::Closed)			// zamkniêcie okna
		{
			window.close();
		}
		if (event.mouseButton.button == sf::Mouse::Left && podniesiona == false)
		{
			sf::Vector2f pos_myszki = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
			for (auto itr = figury.begin(); itr != figury.end(); itr++)
			{
				if ((*itr).czyWSrodku(pos_myszki))
				{
					(*itr).wybor = true;
					mapa[(*itr).x][(*itr).y].zajete = false;
					podniesiona = true;
				}
				else
					(*itr).wybor = false;
			}
		}
		if (event.type == sf::Event::MouseMoved && podniesiona == true)
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
		if (event.mouseButton.button == sf::Mouse::Right && podniesiona == true)
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
							if ((*itr).czyWSrodku(pola[i][j].getPosition()) && !mapa[i][j].zajete)
							{
								(*itr).setPosition(pola[i][j].getPosition());
								(*itr).wybor = false;
								(*itr).x = i;
								(*itr).y = j;
								mapa[i][j].zajete = true;
								mapa[i][j].kolor = (*itr).kolor;
								mapa[i][j].rodzaj_figury = (*itr).rodzaj;
								podniesiona = false;
							}
							else if ((*itr).czyWSrodku(pola[i][j].getPosition()) && mapa[i][j].zajete && (mapa[i][j].kolor != (*itr).kolor))
							{
								for (auto itr2 = figury.begin(); itr2 != figury.end(); itr2++)
								{
									if (itr2 != itr && (*itr2).x == i && (*itr2).y == j)
									{
										(*itr).wybor = false;
										(*itr).x = i;
										(*itr).y = j;
										(*itr2).zbij();
										mapa[i][j].zajete = true;
										mapa[i][j].rodzaj_figury = (*itr).rodzaj;
										mapa[i][j].kolor = (*itr).kolor;
										podniesiona = false;
										(*itr).setPosition(pola[i][j].getPosition());
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void tekst()
{
	sf::Text text;

	// wybranie czcionki
	text.setFont(font);

	// ustawienie stringa który ma byæ wyœwietlony
	text.setString("Test tekstu");

	// ustawienie wielkoœci czcionki
	text.setCharacterSize(70); // w pikselach, nie punktach!
	text.setPosition(1100, 300);
	// ustawienie stylu tekstu
	text.setStyle(sf::Text::Bold | sf::Text::Italic);

	// wewn¹trz g³ównej pêtli, pomiêdzy window.clear(), a window.display()
	window.draw(text);
}

void rysowanie()
{
	window.clear(sf::Color::Black);		// czyszczenie okna na czarny kolor (nie trzeba podawaæ argumentu, standardowo czyœci siê na czarny)
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			window.draw(pola[i][j]);
		}
	}
	for (auto itr = figury.begin(); itr != figury.end(); itr++)
	{
		(*itr).draw(window, sf::RenderStates::Default);
		(*itr).wczytajSprite();
	}
	tekst();
	window.display();					// koniec tej klatki
}

void tworzenieFigur()
{
	for (int i = 0; i < 8; i++)
	{
		Figura bialy_pion(pionek, bialy, pola[i][6].getPosition());
		bialy_pion.x = i;
		bialy_pion.y = 6;
		figury.push_back(bialy_pion);
		mapa[i][6].zajete = true;
		mapa[i][6].kolor = bialy;
		mapa[i][6].rodzaj_figury = pionek;

		Figura czarny_pion(pionek, czarny, pola[i][1].getPosition());
		czarny_pion.x = i;
		czarny_pion.y = 1;
		figury.push_back(czarny_pion);
		mapa[i][1].zajete = true;
		mapa[i][1].kolor = czarny;
		mapa[i][1].rodzaj_figury = pionek;
	}

	for (int i = 0; i < 2; i++)
	{
		Figura biala_wieza(wieza, bialy, pola[0 + i * 7][7].getPosition());
		biala_wieza.x = 0 + i * 7;
		biala_wieza.y = 7;
		figury.push_back(biala_wieza);
		mapa[0 + i * 7][7].zajete = true;
		mapa[0 + i * 7][7].kolor = bialy;
		mapa[0 + i * 7][7].rodzaj_figury = wieza;

		Figura czarna_wieza(wieza, czarny, pola[0 + i * 7][0].getPosition());
		czarna_wieza.x = 0 + i * 7;
		czarna_wieza.y = 0;
		figury.push_back(czarna_wieza);
		mapa[0 + i * 7][0].zajete = true;
		mapa[0 + i * 7][0].kolor = czarny;
		mapa[0 + i * 7][0].rodzaj_figury = wieza;

		Figura bialy_skoczek(skoczek, bialy, pola[1 + i * 5][7].getPosition());
		bialy_skoczek.x = 1 + i * 5;
		bialy_skoczek.y = 7;
		figury.push_back(bialy_skoczek);
		mapa[1 + i * 5][7].zajete = true;
		mapa[1 + i * 5][7].kolor = bialy;
		mapa[1 + i * 5][7].rodzaj_figury = skoczek;

		Figura czarny_skoczek(skoczek, czarny, pola[1 + i * 5][0].getPosition());
		czarny_skoczek.x = 1 + i * 5;
		czarny_skoczek.y = 0;
		figury.push_back(czarny_skoczek);
		mapa[1 + i * 5][0].zajete = true;
		mapa[1 + i * 5][0].kolor = czarny;
		mapa[1 + i * 5][0].rodzaj_figury = skoczek;

		Figura bialy_goniec(goniec, bialy, pola[2 + i * 3][7].getPosition());
		bialy_goniec.x = 2 + i * 3;
		bialy_goniec.y = 7;
		figury.push_back(bialy_goniec);
		mapa[2 + i * 3][7].zajete = true;
		mapa[2 + i * 3][7].kolor = bialy;
		mapa[2 + i * 3][7].rodzaj_figury = goniec;

		Figura czarny_goniec(goniec, czarny, pola[2 + i * 3][0].getPosition());
		czarny_goniec.x = 2 + i * 3;
		czarny_goniec.y = 0;
		figury.push_back(czarny_goniec);
		mapa[2 + i * 3][0].zajete = true;
		mapa[2 + i * 3][0].kolor = czarny;
		mapa[2 + i * 3][0].rodzaj_figury = goniec;
	}

	Figura czarny_hetman(hetman, czarny, pola[3][0].getPosition());
	czarny_hetman.x = 3;
	czarny_hetman.y = 0;
	figury.push_back(czarny_hetman);
	mapa[3][0].zajete = true;
	mapa[3][0].kolor = czarny;
	mapa[3][0].rodzaj_figury = hetman;

	Figura czarny_krol(krol, czarny, pola[4][0].getPosition());
	czarny_krol.x = 4;
	czarny_krol.y = 0;
	figury.push_back(czarny_krol);
	mapa[4][0].zajete = true;
	mapa[4][0].kolor = czarny;
	mapa[4][0].rodzaj_figury = krol;

	Figura bialy_hetman(hetman, bialy, pola[3][7].getPosition());
	bialy_hetman.x = 3;
	bialy_hetman.y = 7;
	figury.push_back(bialy_hetman);
	mapa[3][7].zajete = true;
	mapa[3][7].kolor = bialy;
	mapa[3][7].rodzaj_figury = hetman;

	Figura bialy_krol(krol, bialy, pola[4][7].getPosition());
	bialy_krol.x = 4;
	bialy_krol.y = 7;
	figury.push_back(bialy_krol);
	mapa[4][7].zajete = true;
	mapa[4][7].kolor = bialy;
	mapa[4][7].rodzaj_figury = krol;
}

void siatkaStworz()
{
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
}

void tworzeniePlanszy()
{
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

int main()
{
	tworzeniePlanszy();
	font.loadFromFile("czcionka.ttf");
	ustawieniaOkna();
	siatkaStworz();
	tworzenieFigur();
	while (window.isOpen())						// program bêdzie w³¹czony dopóki okno jest otwarte
	{
		obslugaKlawiaturyIMyszy();
		rysowanie();
	}

	return 0;
}