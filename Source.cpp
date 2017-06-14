#include <SFML/Graphics.hpp>
#include "Connector.hpp"
#include "Figura.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <time.h>

/*Wymiary okna:*/
#define szerokosc_okna 1600
#define wysokosc_okna 1000
#define rozmiar_pola 110
/*_____________*/

sf::ContextSettings settings;
sf::RenderWindow window(sf::VideoMode(szerokosc_okna, wysokosc_okna), "Szachy", sf::Style::Close, settings);

struct Siatka
{
	RodzajFigury rodzaj_figury;
	bool zajete = false;
	Kolor kolor;
};

/*Globalne*/
std::vector <std::vector<sf::RectangleShape>>pola;	//do rysowania planszy
std::vector <std::vector<Siatka>>mapa;				//do logiki gry
std::vector <Figura> figury;

bool podniesiona = false;
bool komputer = true;								//czy grac z Stockfishem??
bool start = true;

Kolor tura = bialy;									//poczatkowa tura
Kolor kolor_komputera = czarny;

std::string nowa_pozycja = "";
std::string komp_pozycja = "";
sf::Texture textura_przegrana;
sf::Texture textura_poczatek;
sf::Texture textura_zasady;
sf::Sprite sprite;
sf::Font font;
/*_________*/

void ustawieniaOkna()
{
	//window.setVerticalSyncEnabled(true); //vsync
	window.setFramerateLimit(60); // argument: ilość fps (nigdy nie używaj obu na raz)
}

bool logik(int _x, int _y, Figura &fig)
{
	switch (fig.rodzaj)
	{
	case pionek:
	{
		//Wymiana na hetmana:
		if (fig.kolor == czarny && _y == 7)
		{
			fig.rodzaj = hetman;
			mapa[_x][_y].rodzaj_figury = hetman;
			fig.naHetmana();
		}
		else if (fig.kolor == bialy && _y == 0)
		{
			fig.rodzaj = hetman;
			mapa[_x][_y].rodzaj_figury = hetman;
			fig.naHetmana();
		}
		//#################
		int pomocnicza = 1;
		if (fig.kolor == czarny) pomocnicza = -1;
		if ((_y == fig.y - 1 * pomocnicza || (_y == fig.y - 2 * pomocnicza && fig.licznik_poruszania == 0)) && (_x == fig.x) && (!mapa[_x][_y].zajete))
		{
			return true;
		}
		else if (mapa[_x][_y].zajete && (_x == fig.x + 1 || _x == fig.x - 1) && _y == fig.y - 1 * pomocnicza)
		{
			return true;
		}
		//Bicie w przelocie:
		else if ((_x == fig.x + 1 || _x == fig.x - 1) && _y == fig.y - 1 * pomocnicza && !mapa[_x][_y].zajete && mapa[_x][_y + pomocnicza].zajete && mapa[_x][_y + pomocnicza].rodzaj_figury == pionek && mapa[_x][_y + pomocnicza].kolor != fig.kolor)
		{
			for (auto itr3 = figury.begin(); itr3 != figury.end(); itr3++)
			{
				if ((*itr3).kolor != fig.kolor && (*itr3).rodzaj == pionek && (*itr3).licznik_poruszania == 1 && (*itr3).x == _x && (*itr3).y == _y + pomocnicza)
				{
					std::cout << "Bicie w przelocie";
					(*itr3).zbij();
					mapa[_x][_y + pomocnicza].zajete = false;
					mapa[_x][_y + pomocnicza].rodzaj_figury = nic;
				}
			}
			return true;
		}
		//################
		else if (_x == fig.x && _y == fig.y)
		{
			//return true;
			return false;
		}
		else
			return false;
	}
	case skoczek:
	{
		if ((_x == fig.x + 1) && (_y == fig.y + 2 || _y == fig.y - 2))
		{
			return true;
		}
		else if ((_x == fig.x - 1) && (_y == fig.y + 2 || _y == fig.y - 2))
		{
			return true;
		}
		else if ((_y == fig.y + 1) && (_x == fig.x + 2 || _x == fig.x - 2))
		{
			return true;
		}
		else if ((_y == fig.y - 1) && (_x == fig.x + 2 || _x == fig.x - 2))
		{
			return true;
		}
		else if (_x == fig.x && _y == fig.y)
		{
			//return true;
			return false;
		}
		else
			return false;
	}
	case goniec:
	{
		for (int i = 1; i < 9; i++)
		{
			if (_x == fig.x + i && _y == fig.y + i)
			{
				for (int k = 1; k < _x - fig.x; k++)
				{
					if (mapa[_x - k][_y - k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x + i && _y == fig.y - i)
			{
				for (int k = 1; k < _x - fig.x; k++)
				{
					if (mapa[_x - k][_y + k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x - i && _y == fig.y - i)
			{
				for (int k = 1; k < fig.x - _x; k++)
				{
					if (mapa[_x + k][_y + k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x - i && _y == fig.y + i)
			{
				for (int k = 1; k < fig.x - _x; k++)
				{
					if (mapa[_x + k][_y - k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y)
			{
				//return true;
				return false;
			}
		}
		return false;
	}
	case wieza:
	{
		for (int i = 1; i < 9; i++)
		{
			if (_x == fig.x + i && _y == fig.y)
			{
				for (int k = 1; k < _x - fig.x; k++)
				{
					if (mapa[_x - k][_y].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x - i && _y == fig.y)
			{
				for (int k = 1; k < fig.x - _x; k++)
				{
					if (mapa[_x + k][_y].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y + i)
			{
				for (int k = 1; k < _y - fig.y; k++)
				{
					if (mapa[_x][_y - k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y - i)
			{
				for (int k = 1; k < fig.y - _y; k++)
				{
					if (mapa[_x][_y + k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y)
			{
				//return true;
				return false;
			}
		}
		return false;
	}
	case hetman:
	{
		for (int i = 1; i < 9; i++)
		{
			if (_x == fig.x + i && _y == fig.y)
			{
				for (int k = 1; k < _x - fig.x; k++)
				{
					if (mapa[_x - k][_y].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x - i && _y == fig.y)
			{
				for (int k = 1; k < fig.x - _x; k++)
				{
					if (mapa[_x + k][_y].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y + i)
			{
				for (int k = 1; k < _y - fig.y; k++)
				{
					if (mapa[_x][_y - k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y - i)
			{
				for (int k = 1; k < fig.y - _y; k++)
				{
					if (mapa[_x][_y + k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y)
			{
				//return true;
				return false;
			}
		}
		for (int i = 1; i < 9; i++)
		{
			if (_x == fig.x + i && _y == fig.y + i)
			{
				for (int k = 1; k < _x - fig.x; k++)
				{
					if (mapa[_x - k][_y - k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x + i && _y == fig.y - i)
			{
				for (int k = 1; k < _x - fig.x; k++)
				{
					if (mapa[_x - k][_y + k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x - i && _y == fig.y - i)
			{
				for (int k = 1; k < fig.x - _x; k++)
				{
					if (mapa[_x + k][_y + k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x - i && _y == fig.y + i)
			{
				for (int k = 1; k < fig.x - _x; k++)
				{
					if (mapa[_x + k][_y - k].zajete)
					{
						return false;
					}
				}
				return true;
			}
			else if (_x == fig.x && _y == fig.y)
			{
				//return true;
				return false;
			}
		}
		return false;
	}
	case krol:
	{
		if (_x == fig.x && _y == fig.y) return false;
		else if ((_x == fig.x + 1 || _x == fig.x - 1 || _x == fig.x) && (_y == fig.y + 1 || _y == fig.y - 1 || _y == fig.y) && (!mapa[_x][_y].zajete))
		{
			return true;
		}
		else if (mapa[_x][_y].zajete && (_x == fig.x + 1 || _x == fig.x - 1 || _x == fig.x) && (_y == fig.y + 1 || _y == fig.y - 1 || _y == fig.y))
		{
			return true;
		}
		//Roszady:
		else if (fig.licznik_poruszania == 0 && _x == 6 && _y == fig.y &&  mapa[7][_y].rodzaj_figury == wieza && !mapa[6][_y].zajete && !mapa[5][_y].zajete)
		{
			for (auto itr3 = figury.begin(); itr3 != figury.end(); itr3++)
			{
				if ((*itr3).kolor == fig.kolor && (*itr3).rodzaj == wieza && (*itr3).licznik_poruszania == 0 && (*itr3).x == 7 && (*itr3).y == _y)
				{
					std::cout << "Roszada w prawo";
					mapa[(*itr3).x][(*itr3).y].zajete = false;
					mapa[(*itr3).x][(*itr3).y].rodzaj_figury = nic;
					mapa[5][_y].zajete = true;
					mapa[5][_y].kolor = (*itr3).kolor;
					mapa[5][_y].rodzaj_figury == wieza;
					(*itr3).x = 5;
					(*itr3).licznik_poruszania++;
					(*itr3).setPosition(pola[5][_y].getPosition());
					podniesiona == false;
					fig.wybor == false;
				}
			}
			return true;
		}
		else if (fig.licznik_poruszania == 0 && _x == 2 && _y == fig.y && !mapa[1][_y].zajete && !mapa[2][_y].zajete && !mapa[3][_y].zajete && mapa[0][_y].rodzaj_figury == wieza)
		{
			for (auto itr3 = figury.begin(); itr3 != figury.end(); itr3++)
			{
				if ((*itr3).kolor == fig.kolor && (*itr3).rodzaj == wieza && (*itr3).licznik_poruszania == 0 && (*itr3).x == 0 && (*itr3).y == _y)
				{
					std::cout << "Roszada w lewo";
					mapa[(*itr3).x][(*itr3).y].zajete = false;
					mapa[(*itr3).x][(*itr3).y].rodzaj_figury = nic;
					mapa[3][_y].zajete = true;
					mapa[3][_y].kolor = (*itr3).kolor;
					mapa[3][_y].rodzaj_figury == wieza;
					(*itr3).x = 3;
					(*itr3).licznik_poruszania++;
					(*itr3).setPosition(pola[3][_y].getPosition());
					podniesiona == false;
					fig.wybor == false;
				}
			}
			return true;
		}
		//#############
		else
			return false;
	}
	}
}

std::string naNotacje(int x, int y)
{
	std::string s = "";
	s += char('a' + x);
	s += char('8' - y);
	return s;
}

void tekst(std::string te)
{
	sf::Text text;

	// wybranie czcionki
	text.setFont(font);

	// ustawienie stringa który ma byæ wyœwietlony
	text.setString(te);

	// ustawienie wielkoœci czcionki
	text.setCharacterSize(35); // w pikselach, nie punktach!
	text.setPosition(1100, 300);
	// ustawienie stylu tekstu
	text.setStyle(sf::Text::Bold | sf::Text::Italic);

	// wewn¹trz g³ównej pêtli, pomiêdzy window.clear(), a window.display()
	window.draw(text);
}

void ruchStockfisha(std::string notacja)
{
	std::system("cls");

	std::cout << "Komputer ruszyl sie: " << komp_pozycja << "TWOJ RUCH !!" << std::endl;
	nowa_pozycja += komp_pozycja + " ";
	int stare_x = int(notacja[0]) - 97;
	int stare_y = abs(int(notacja[1]) - 56);

	int nowe_x = int(notacja[2]) - 97;
	int nowe_y = abs(int(notacja[3]) - 56);

	for (auto itr = figury.begin(); itr != figury.end(); itr++)
	{
		if ((*itr).x == stare_x && (*itr).y == stare_y)
		{
			(*itr).wybor = true;
			mapa[(*itr).x][(*itr).y].zajete = false;
			mapa[(*itr).x][(*itr).y].rodzaj_figury = nic;
			podniesiona = true;
			if (!mapa[nowe_x][nowe_y].zajete)
			{
				logik(nowe_x, nowe_y, (*itr));
				(*itr).setPosition(pola[nowe_x][nowe_y].getPosition());
				(*itr).licznik_poruszania++;
				(*itr).wybor = false;
				(*itr).x = nowe_x;
				(*itr).y = nowe_y;
				mapa[nowe_x][nowe_y].zajete = true;
				mapa[nowe_x][nowe_y].kolor = (*itr).kolor;
				mapa[nowe_x][nowe_y].rodzaj_figury = (*itr).rodzaj;
				podniesiona = false;
				if (tura == czarny)tura = bialy;
				else tura = czarny;
			}
			else if (mapa[nowe_x][nowe_y].zajete)
			{
				for (auto itr2 = figury.begin(); itr2 != figury.end(); itr2++)
				{
					if (itr2 != itr && (*itr2).x == nowe_x && (*itr2).y == nowe_y)
					{
						logik(nowe_x, nowe_y, (*itr));
						(*itr).wybor = false;
						(*itr).x = nowe_x;
						(*itr).y = nowe_y;
						(*itr2).zbij();
						mapa[nowe_x][nowe_y].zajete = true;
						mapa[nowe_x][nowe_y].rodzaj_figury = (*itr).rodzaj;
						mapa[nowe_x][nowe_y].kolor = (*itr).kolor;
						podniesiona = false;
						(*itr).setPosition(pola[nowe_x][nowe_y].getPosition());
						(*itr).licznik_poruszania++;
						if (tura == czarny)tura = bialy;
						else tura = czarny;
					}
				}
			}
		}
	}
}

void obslugaKlawiaturyIMyszy()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)			//zamkniecie okna
		{
			window.close();
		}
		if (event.key.code == sf::Keyboard::B && start)
		{
			kolor_komputera = czarny;
			start = false;
		}
		else if (event.key.code == sf::Keyboard::C && start) {
			kolor_komputera = bialy;
			start = false;
		}
		if (!start)
		{
			//PODNOSZENIE
			if (tura == kolor_komputera && komputer == true)
			{
				komp_pozycja = getNextMove(nowa_pozycja);
				ruchStockfisha(komp_pozycja);
			}
			else
			{
				if (event.mouseButton.button == sf::Mouse::Left && podniesiona == false)
				{
					sf::Vector2f pos_myszki = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
					for (auto itr = figury.begin(); itr != figury.end(); itr++)
					{
						if ((*itr).czyWSrodku(pos_myszki) && (*itr).kolor == tura)
						{
							(*itr).wybor = true;
							mapa[(*itr).x][(*itr).y].zajete = false;
							mapa[(*itr).x][(*itr).y].rodzaj_figury = nic;
							podniesiona = true;
							nowa_pozycja += naNotacje((*itr).x, (*itr).y);
						}
						else
							(*itr).wybor = false;
					}
				}
				//PRZESUWANIE
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
				//USTAWIANIE
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
									if ((*itr).czyWSrodku(pola[i][j].getPosition()) && !mapa[i][j].zajete && logik(i, j, (*itr)))
									{
										(*itr).setPosition(pola[i][j].getPosition());
										(*itr).licznik_poruszania++;
										(*itr).wybor = false;
										(*itr).x = i;
										(*itr).y = j;
										mapa[i][j].zajete = true;
										mapa[i][j].kolor = (*itr).kolor;
										mapa[i][j].rodzaj_figury = (*itr).rodzaj;
										podniesiona = false;
										nowa_pozycja += naNotacje((*itr).x, (*itr).y) + " ";

										if (tura == bialy) tura = czarny;
										else tura = bialy;
										/*komp_pozycja = getNextMove(nowa_pozycja);
										if (komp_pozycja == "error") std::cout << "error";
										std::cout << komp_pozycja << std::endl;*/
									}
									else if ((*itr).czyWSrodku(pola[i][j].getPosition()) && mapa[i][j].zajete && logik(i, j, (*itr)) && (mapa[i][j].kolor != (*itr).kolor))
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
												(*itr).licznik_poruszania++;
												nowa_pozycja += naNotacje((*itr).x, (*itr).y) + " ";

												if (tura == bialy) tura = czarny;
												else tura = bialy;
												/*komp_pozycja = getNextMove(nowa_pozycja);
												if (komp_pozycja == "error") std::cout << "error";
												std::cout << komp_pozycja << std::endl;*/
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
	}
}

void rysowanie()
{
	window.clear(sf::Color::Black);		// czyszczenie okna na czarny kolor (nie trzeba podawać argumentu, standardowo czyści się na czarny)
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
	tekst("Komputer ruszyl sie: \n" + komp_pozycja + "\nTWOJ RUCH !!");
	//tekst("LPM - podnies \n PPM - upusc", 1100, 300);
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

void przegrales()
{
	window.clear(sf::Color::Black);
	sprite.setTexture(textura_przegrana);
	window.draw(sprite);
	window.display();
}

void wstep()
{
	window.clear(sf::Color::Black);
	sprite.setTexture(textura_poczatek);
	window.draw(sprite);
	window.display();
	Sleep(4000);
}

void zasady()
{
	window.clear(sf::Color::Black);
	sprite.setTexture(textura_zasady);
	window.draw(sprite);
	window.display();
}

int main()
{
	font.loadFromFile("cs.ttf");
	textura_przegrana.loadFromFile("przegrales.png");
	textura_poczatek.loadFromFile("wejscie.jpg");
	textura_zasady.loadFromFile("forsbite.jpg");
	wstep();
	Sleep(2000);
	tworzeniePlanszy();
	ustawieniaOkna();
	siatkaStworz();
	tworzenieFigur();

	bool k = false;

	ConnectToEngine("stockfish_8_x32.exe");

	while (window.isOpen())						// program będzie włączony dopóki okno jest otwarte
	{
		obslugaKlawiaturyIMyszy();
		while (start)
		{
			obslugaKlawiaturyIMyszy();
			zasady();
		}
		if (koniec == true)
		{
			if (!k)
			{
				rysowanie();
				Sleep(2000);
			}
			k = true;
			przegrales();
		}
		else
		{
			rysowanie();
		}
	}
	CloseConnection();
	return 0;
}