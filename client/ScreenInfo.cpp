#include "ScreenInfo.h"
#include  <iostream>

const unsigned LOGO_SIZE = 100;
const unsigned PADDING = 50;


/*************************************************************************************************************/
//										 Logo Screen
//============================================================================================================
//										logo	constructor
//===============================================================================================================
Logo::Logo(const sf::Font& font) : sf::Text::Text("Agar.io", font, LOGO_SIZE) {
	setFillColor(sf::Color::White);
	setOutlineThickness(3.f);
	setOutlineColor(sf::Color(255, 255, 255, 100));//White
	auto x = (float(SCREEN_WIDTH) - getGlobalBounds().width) / 2;
	auto y = (float(SCREEN_HEIGHT) - getGlobalBounds().height) / 3;

	setPosition(sf::Vector2f{ x,y });
}
/*************************************************************************************************************/
//										 Settings Screen
//============================================================================================================
//									constructor
//===============================================================================================================
SettingsScreen::SettingsScreen(const sf::Font& font, const Images& image) {
	m_currentImage = rand()% (Images_t::CIRCLE_NUMBER-1);

	setNameBox();
	setTextInNameBox(font);

	setCircle(image);

	setTriangle(m_circle[m_currentImage], m_triangleLeft);
	setTriangle(m_circle[m_currentImage], m_triangleRight);
	setTriangleLeft(m_circle[m_currentImage]);
	setTriangleRigh(m_circle[m_currentImage]);
}
//===============================================================================================================
void SettingsScreen::mouseEventButton(const sf::Vector2f& ver, bool p) {
	if (p) {
		if (m_triangleLeft.setPressed(ver))
			m_currentImage = (m_currentImage != 0)? m_currentImage - 1: Images_t::CIRCLE_NUMBER - 2;
		if (m_triangleRight.setPressed(ver))
			m_currentImage = (m_currentImage != Images_t::CIRCLE_NUMBER - 2) ? m_currentImage + 1 : 0;
	}
	else {
		m_triangleLeft.setHover(ver);
		m_triangleRight.setHover(ver);
	}
}

//===============================================================================================================
//								images in circle
//===============================================================================================================
void SettingsScreen::setCircle(const Images& image) {
	for (size_t i = 0; i < Images_t::CIRCLE_NUMBER - 1; ++i) {
		m_circle[i].setRadius(150);
		m_circle[i].setOutlineThickness(5);
		m_circle[i].setOutlineColor(sf::Color::Black);
		auto x = (float(SCREEN_WIDTH) - m_circle[i].getGlobalBounds().width) / 2;
		auto y = (float(SCREEN_HEIGHT) - m_circle[i].getGlobalBounds().height) / 2;
		m_circle[i].setPosition(sf::Vector2f{ x,y });
		m_circle[i].setTexture(&image[sf::Uint32(i+2)]);
	}
}
//===============================================================================================================
//										arrows
//===============================================================================================================
void SettingsScreen::setTriangle(const sf::CircleShape& circle, sf::CircleShape& triangle) {
	triangle.setPointCount(3);
	triangle.setRadius(circle.getRadius() / 4);
	triangle.setFillColor(sf::Color(58, 183, 149));
	triangle.setOutlineThickness(5);
	triangle.setOutlineColor(sf::Color::Black);
}
//===============================================================================================================

void SettingsScreen::setTriangleLeft(const sf::CircleShape& circle) {

	m_triangleLeft.setRotation(270);
	auto x = circle.getPosition().x - PADDING - m_triangleLeft.getRadius();
	auto y = circle.getPosition().y + circle.getRadius() + (m_triangleLeft.getRadius());
	m_triangleLeft.setPosition(x, y);
}
//===============================================================================================================

void SettingsScreen::setTriangleRigh(const sf::CircleShape& circle) {

	m_triangleRight.setRotation(90);
	auto x = circle.getPosition().x + PADDING + circle.getGlobalBounds().width - m_triangleRight.getGlobalBounds().left / 2;
	auto y = circle.getPosition().y + circle.getRadius() / 2 + (m_triangleRight.getRadius()) ;
	m_triangleRight.setPosition(x, y);
}
//=============================================================================================================
//								name box
//===============================================================================================
void SettingsScreen::setNameBox() {
	m_nameBox.setSize(sf::Vector2f{ 250, 40 });
	auto x = (float(SCREEN_WIDTH) - m_nameBox.getGlobalBounds().width) / 2;
	auto y = (float(SCREEN_HEIGHT) - m_nameBox.getGlobalBounds().height) / 8;
	m_nameBox.setPosition(sf::Vector2f{ x,y });
	m_nameBox.setFillColor(sf::Color::White);
	m_nameBox.setOutlineThickness(5);
	m_nameBox.setOutlineColor(sf::Color::Black);
}
//===============================================================================================
//								name text
//===============================================================================================
void SettingsScreen::setTextInNameBox(const sf::Font& font) {
	m_defaultStr = "Enter your name.. ";
	m_name.clear();

	m_theName.setFont(font);
	m_textDef.setFont(font);

	m_textDef.setCharacterSize(30);
	m_theName.setCharacterSize(m_textDef.getCharacterSize());

	m_theName.setFillColor(sf::Color::Black);
	m_textDef.setFillColor(sf::Color(181, 181, 181));//grey

	auto x = m_nameBox.getPosition().x + 5;
	auto y = m_nameBox.getPosition().y - 5;
	m_textDef.setPosition(sf::Vector2f{ x,y });
	m_theName.setPosition(m_textDef.getPosition());

	m_theName.setString(m_name);
	m_textDef.setString(m_defaultStr);
}
//===============================================================================================================
//						when text is enterd
//=========================================================================================
void SettingsScreen::enterName(sf::Event& event) {

	if (event.text.unicode >= 32 && event.text.unicode <= 126)
		if (m_nameBox.getGlobalBounds().width > m_theName.getGlobalBounds().width + 30)
			m_name += char(event.text.unicode);
		else;
	else if (event.text.unicode == 8 && m_name.getSize() > 0)
		m_name.erase(m_name.getSize() - 1, m_name.getSize());

	m_theName.setString(m_name);

}

//===============================================================================================
bool SettingsScreen::pressed(const sf::Vector2f& ver) {
	return (m_nameBox.getGlobalBounds().contains(ver));
}
//===============================================================================================
//								display
//===============================================================================================
void SettingsScreen::display(sf::RenderWindow& w) {
	w.draw(m_nameBox);
	w.draw((m_name.isEmpty()) ? m_textDef : m_theName);
	m_triangleLeft.display(w);
	m_triangleRight.display(w);
	w.draw(m_circle[m_currentImage]);
	
}

/*************************************************************************************************************/
//										 Help Screen
//===============================================================================
HelpScreen::HelpScreen(const sf::Font& font,float padding_left) : sf::Text::Text(helpString(), font, LOGO_SIZE) {

	setFillColor(sf::Color::White);
	setCharacterSize(15);

	setPosition(sf::Vector2f{ padding_left + PADDING,0 });
}
//==================================================================================
sf::String helpString() {
	sf::String str;
	str = "\nGame rulse: \n\n";
	str += "rufufufuf fufuf fuuf ufuf fufuuf ufufu\n\n";
	str += "\n\n\n";
	str += "bla bla bla bla bla bla bla \n\n";
	str += "bla bla bla bla \n\n";
	str += "ga ga ga ga aga aga aga aga ga ag ag aga gagag ag\n\n";
	str += "rufufufuf fufuf fuuf ufuf fufuuf ufufu\n\n";
	str += "bla bla bla bla bla bla bla bla\n\n";
	str += "bla bla bla bla bla bla bla bla bla bla bla\n\n";


	return str;
}