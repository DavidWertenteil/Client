#include "Fonts.h"


Fonts::Fonts()
{

	for (size_t i = 0; i < fonts::NUMBRE_OF_FONTS; ++i) {
		push_back(sf::Font{});
	}
	
	(*this)[LOGO].loadFromFile("fonts/Wonderbar Demo.otf");
	(*this)[MENU].loadFromFile("fonts/comic.ttf");
	(*this)[SETTINGS].loadFromFile("fonts/Sunny Spring Day - OTF.otf");
	(*this)[HELP].loadFromFile("fonts/Kid Marker.ttf");
	(*this)[HELP_INFO].loadFromFile("fonts/help.ttf");
	(*this)[NAME].loadFromFile("fonts/name.ttf");
	(*this)[SCORE].loadFromFile("fonts/score.otf");

	
}


Fonts::~Fonts()
{
}
