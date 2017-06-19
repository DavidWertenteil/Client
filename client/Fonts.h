#pragma once
#include <SFML\Graphics.hpp>

enum fonts {
	LOGO,
	MENU,
	SETTINGS,
	HELP,
	NAME,
	SCORE,
	NUMBRE_OF_FONTS
};

class Fonts:public std::vector<sf::Font>
{
public:
	Fonts();
	~Fonts();
};

