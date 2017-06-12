#pragma once
#include "Fonts.h"
#include "Images.h"


class ScreenInfo
{
public:
	ScreenInfo() = default;
	virtual ~ScreenInfo() = default;
	virtual void display(sf::RenderWindow&) = 0;
	virtual void mouseEventButton(const sf::Vector2f&, bool) {}

};
//=======================================================================================
//						class logo
//
//		 desplay the logo of the game by default
//=======================================================================================
class Logo :public sf::Text, public ScreenInfo {
public:
	Logo(const sf::Font&);
	~Logo() = default;
	void display(sf::RenderWindow& w)override { w.draw((*this)); }
};
//=======================================================================================
//                class  Help Screen - display the game info
//=======================================================================================
class HelpScreen :public sf::Text, public ScreenInfo {
public:
	HelpScreen(const sf::Font&, float);
	~HelpScreen() = default;
	void display(sf::RenderWindow& w)override { w.draw((*this)); }

};

sf::String helpString();
//======================================================================================
//               class settings screen
//======================================================================================
//========================================================================================================
class Triangl : public sf::CircleShape {
public:
	Triangl() :m_hover{ false }, m_pressed{ false } {}
	~Triangl() {}
	bool getHover()const { return m_hover; }
	bool getPressed()const { return m_pressed; }

	void setHover(const sf::Vector2f& ver) { m_hover = getGlobalBounds().contains(ver); }
	bool setPressed(const sf::Vector2f& ver) { return(m_pressed = getGlobalBounds().contains(ver)); }

	void display(sf::RenderWindow& w) { setFillColor(sf::Color(58, 100, 149, (m_hover) ? 150 : 255)); w.draw((*this)); }

private:
	bool m_hover;
	bool m_pressed;
};
//============================================================================================================

//========================================================================================================
class SettingsScreen : public ScreenInfo {
public:
	SettingsScreen(const sf::Font&, const Images&);
	~SettingsScreen() = default;
	void display(sf::RenderWindow& w)override;
	bool pressed(const sf::Vector2f&);
	void enterName(sf::Event&);
	void mouseEventButton(const sf::Vector2f&, bool)override;

	sf::Uint32 getSelectedImage()const { return m_currentImage; }
	const sf::String& getName()const { return m_theName.getString(); }

private:
	/*text box for users name*/
	//begin :
	void setNameBox();
	void setTextInNameBox(const sf::Font&);

	sf::RectangleShape m_nameBox;
	sf::Text m_textDef;
	sf::Text m_theName;
	sf::String m_defaultStr;
	sf::String m_name;
	//end.

	/*select users image*/
	//begin :
	Triangl m_triangleLeft;
	Triangl m_triangleRight;
	sf::CircleShape m_circle[Images_t::CIRCLE_NUMBER - 1];

	void setCircle(const Images& image);
	void setTriangle(const sf::CircleShape&, sf::CircleShape&);
	void setTriangleLeft(const sf::CircleShape&);
	void setTriangleRigh(const sf::CircleShape&);
	//end.

	sf::Uint32 m_currentImage;


};

