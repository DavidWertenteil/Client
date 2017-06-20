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
	inline static Fonts& instance() { static Fonts tone; return tone; }
	~Fonts();
private:
	Fonts();
	Fonts(const Fonts&) = delete;
	Fonts& operator=(const Fonts&) = delete;

};

