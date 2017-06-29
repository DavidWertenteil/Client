#pragma once
#include "tempScreens.h"
#include "ScreenInfo.h"

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
		:sf::Text::Text(str, font, FONT_SIZE) {}
	virtual ~Button() = 0;


	/* check functions */
	bool check(const sf::Vector2f& location) const { return (*this).getGlobalBounds().contains(location); }
	virtual void onBotton(const sf::Vector2f& location) { m_switch = check(location); }
	virtual void display(sf::RenderWindow& window) { setFillColor((m_switch /*|| m_pressed*/) ? display2 : display1); window.draw(*this); }

	/* virtual functions */
	virtual bool pressed(const sf::Vector2f&) = 0;
	virtual bool getPressed()const { return m_pressed; }
	virtual void setPressed(bool p) { m_pressed = p; }

	virtual void selected(sf::RenderWindow&, sf::Event&) = 0;
protected:
	bool m_switch{ false };    //if mouse on button switch display
	bool m_pressed{ false };
};
//======================================================================================
//                           class Start
//======================================================================================
class Start : public Button {
public:
	Start(const sf::String& str = "Play", unsigned p = 0)
		:Button(Fonts::instance()[MENU], str),
		m_load(),
		m_end(),
		m_settings() {}
	bool pressed(const sf::Vector2f& location)override { return(m_pressed = check(location)); }
	void load(sf::RenderWindow& w)const { m_load.display(w); }
	void selected(sf::RenderWindow&, sf::Event&)override;
	void gameOver(sf::RenderWindow& w, sf::View&v, unsigned s) { m_end.endLevelScreen(w, v, s); }
	SettingsScreen& settings() { return std::ref(m_settings); }

private:
	loading m_load;
	EndLevel m_end;
	SettingsScreen m_settings;
};
//======================================================================================
//                           class Settings
//======================================================================================
class Settings : public Button {
public:
	Settings(SettingsScreen& set, const sf::String& str = "Settings") :Button(Fonts::instance()[MENU], str), m_settings(set) {}
	bool pressed(const sf::Vector2f& location)override { return(m_pressed = check(location)); }
	void selected(sf::RenderWindow& w, sf::Event& e)override { m_settings.selected(w, e); }
private:
	SettingsScreen& m_settings;
};
//======================================================================================
//                           class Help
//======================================================================================
class Help : public Button {
public:
	Help(const sf::String& str = "Help") :Button(Fonts::instance()[MENU], str), m_help(Fonts::instance()[fonts::HELP_INFO]) {}
	bool pressed(const sf::Vector2f& location)override { return(m_pressed = check(location)); }
	void selected(sf::RenderWindow& w, sf::Event&)override { m_help.display(w); }
	void setWidth(float width) { m_help.setWidth(width); }
private:
	HelpScreen m_help;
};
//======================================================================================
//                        class Close
//======================================================================================
class Close : public Button {
public:
	Close(const sf::String& str = "Close") :Button(Fonts::instance()[MENU], str), m_logo(Fonts::instance()[fonts::LOGO]) {}
	bool pressed(const sf::Vector2f& location)override;
	void display(sf::RenderWindow& window)override;
	void selected(sf::RenderWindow& w, sf::Event&)override { m_logo.display(w); }
private:
	Logo m_logo;
};

//**************************************************************************************

//**************************************************************************************

class Menu :public std::vector<std::unique_ptr<Button>>, public sf::RectangleShape {
public:
	Menu();
	void mouseEventButton(const sf::Vector2f&, bool);
	auto getIteratorToCurrentPressed()const { return m_itToPressed; }
	const sf::RectangleShape& getRec()const { return (*this); }
	void restartMenu() { if (m_itToPressed == begin()) { (*this)[0]->setPressed(false); restartIt(); } }
	void enter() { m_itToPressed = begin(); }
private:
	void setMenuRec();
	void setMenuVector();

	void restartIt() { m_itToPressed = --end(); }
	std::vector<std::unique_ptr<Button>>::iterator m_itToPressed;

};
