#pragma once
#include "Fonts.h"
#include "Images.h"

class tempScreen :public sf::RectangleShape {
public:
	tempScreen(const sf::Text& title, const sf::Text& info);
	virtual ~tempScreen() = 0;
	virtual void display(sf::RenderWindow&) const;
	void setText(sf::Text&, const sf::Text&);
	void setTextPosition(sf::Text&, float, float);

protected:
	sf::Text m_title;
	sf::Text m_info;
};
//==============================================================================

//===============================================================================
class EndLevel :public tempScreen {
public:
	EndLevel();
	virtual ~EndLevel() = default;
	void endLevelScreen(sf::RenderWindow&, sf::View&, unsigned);
private:
	void setScore(unsigned s) { m_info.setString("Your score is: " + std::to_string(s)); }
};
//==============================================================================

//===============================================================================
class loading :public tempScreen {
public:
	loading();
	virtual ~loading() = default;
};