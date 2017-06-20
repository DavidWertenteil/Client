#include "tempScreens.h"
#include <Windows.h>
tempScreen::~tempScreen() = default;
//===================================================================================
tempScreen::tempScreen(const sf::Text& title,const sf::Text& info)
	:sf::RectangleShape({ float(SCREEN_WIDTH),float(SCREEN_HEIGHT) }),
	m_info(info)
	{
	setFillColor(sf::Color(0, 0, 0, 130));

	setText(m_title, title);
	setText(m_info, info);

	setTextPosition(m_title, 2, 6);
	setTextPosition(m_info, 2, 1.5f);
}
//==================================================================================
void tempScreen::setText(sf::Text& text, const sf::Text& other) {
	text.setString(other.getString());
	text.setFont(*other.getFont());
	text.setCharacterSize(other.getCharacterSize());
}
//==================================================================================
void tempScreen::setTextPosition(sf::Text& text, float factorX, float factorY) {
	auto x = (float(SCREEN_WIDTH) - text.getGlobalBounds().width) / factorX;
	auto y = (float(SCREEN_HEIGHT) - text.getGlobalBounds().height) / factorY;
	text.setPosition({ x, y });
}
//==================================================================================
void tempScreen::display(sf::RenderWindow& w) const{
	w.draw(*this);
	w.draw(m_title);
	w.draw(m_info);
	w.display();
}
//==================================================================================

//==================================================================================
EndLevel::EndLevel() 
	:tempScreen(sf::Text{ "GAME OVER",Fonts::instance()[LOGO],150 },
		sf::Text{ "Your score is: ",Fonts::instance()[SCORE],50 }) {
	m_title.setFillColor(sf::Color::Yellow);
	m_title.setOutlineThickness(8);
	m_title.setOutlineColor(sf::Color(220, 220, 220, 100));
	m_info.setFillColor(sf::Color(255, 182, 193));
}
//==================================================================================
void EndLevel::endLevelScreen(sf::RenderWindow& w, sf::View& v, unsigned score) {
	setScore(score);
	v.setCenter(float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2);
	w.setView(v);
	display(w);
	Sleep(5000);
}
//==================================================================================

//==================================================================================
loading::loading()
	:tempScreen(sf::Text{ "loading",Fonts::instance()[LOGO],80 }, 
		sf::Text{ "plaese wait",Fonts::instance()[NAME],30 }) {
	m_title.setFillColor(sf::Color(190, 190, 190));
	m_info.setFillColor(sf::Color(255, 182, 193));
}

