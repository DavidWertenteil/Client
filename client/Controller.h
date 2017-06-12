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
	void play(sf::RenderWindow&);

private:
	void makeScreen();

	Fonts m_fonts;
	sf::Music m_music;
	Images m_images;   //loads from file and saves in a vector of Sprite
	Menu m_Menus; //display background and menu
	std::vector<std::unique_ptr<ScreenInfo>> m_screeninfo;//the screen information that is displayed by the bar
};
