#pragma once
#include "Images.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Menu.h"
#include "ScreenInfo.h"
#include <SFML/Audio.hpp>
#include "Game.h"

//===========================================
class Controller
{
public:
	Controller();
	virtual ~Controller() {}

	void run();
	void MenuEvents(sf::RenderWindow&);
	void events(sf::RenderWindow&);

private:

	sf::Music m_music;
	Menu m_Menus; //display background and menu
};
