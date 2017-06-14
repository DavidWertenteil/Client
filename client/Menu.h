#pragma once
#include "Images.h"
#include "Fonts.h"

#include <memory>

const sf::Color display1(sf::Color(58, 183, 149));
const sf::Color display2(sf::Color(58, 255, 149));

const unsigned FONT_SIZE = 40;
/**/
const unsigned TOP_PADDING = 60;
const unsigned DOWN_PADDING = 60;
const unsigned LEFT_PADDING = 30;
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
class Button :public sf::Text
{
public:
	Button(const sf::Font& font, const sf::String& str, unsigned p = 0)
		:sf::Text::Text(str, font, FONT_SIZE), m_place(p) {}
	virtual ~Button() = 0;


	/* check functions */
	bool check(const sf::Vector2f& location) const { return (*this).getGlobalBounds().contains(location); }
	virtual void onBotton(const sf::Vector2f& location) { m_switch = check(location); }
	virtual void display(sf::RenderWindow& window) { setFillColor((m_switch /*|| m_pressed*/) ? display2 : display1); window.draw(*this); }

	/* virtual functions */
	virtual bool pressed(const sf::Vector2f&) = 0;
	virtual bool getPressed()const { return m_pressed; }
	virtual void setPressed(bool p) { m_pressed = p; }
	virtual unsigned getPlace()const { return m_place; }


protected:
	bool m_switch{ false };    //if mouse on button switch display
	bool m_pressed{ false };
	unsigned m_place; //its place(cell) in the array
};
//======================================================================================
//                           class Start
//======================================================================================
class Start : public Button {
public:
	Start(const sf::Font& font, const sf::String& str = "Play", unsigned p = 0) :Button(font, str) {}
	bool pressed(const sf::Vector2f& location)override { return(m_pressed = check(location)); }

};
//======================================================================================
//                           class Settings
//======================================================================================
class Settings : public Button {
public:
	Settings(const sf::Font& font, const sf::String& str = "Settings", unsigned p = 1) :Button(font, str, p) {}

	bool pressed(const sf::Vector2f& location)override { return(m_pressed = check(location)); }

};
//======================================================================================
//                           class Help
//======================================================================================
class Help : public Button {
public:
	Help(const sf::Font& font, const sf::String& str = "Help", unsigned p = 2) :Button(font, str, p) {}

	bool pressed(const sf::Vector2f& location)override { return(m_pressed = check(location)); }

};
//======================================================================================
//                        class Close
//======================================================================================
class Close : public Button {
public:
	Close(const sf::Font& font, const sf::String& str = "Close", unsigned p = 3) :Button(font, str) {}
	bool pressed(const sf::Vector2f& location)override;
	void display(sf::RenderWindow& window)override;
};

//**************************************************************************************

//**************************************************************************************

class Menu :public std::vector<std::unique_ptr<Button>>, public sf::RectangleShape {
public:
	Menu(const Fonts&);
	void mouseEventButton(const sf::Vector2f&, bool);
	auto getIteratorToCurrentPressed()const { return m_itToPressed; }
	const sf::RectangleShape& getRec()const { return (*this); }
	void restartIt() { m_itToPressed = --end(); }
private:
	void setMenuRec();
	void setMenuVector(const Fonts&);

	std::vector<std::unique_ptr<Button>>::iterator m_itToPressed;

};
