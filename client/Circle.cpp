#pragma once
#include "Circle.h"

//====================================================================================
//===========================      CONSTRACTORS      =================================
//====================================================================================
Player::Player(Uint32 id, Vector2f c, unsigned s) :Circle(id, c) 
{
	setPointCount(100);
	setOutlineThickness(4);
	setOutlineColor(sf::Color::Black);

	m_score = unsigned(getRadius());
}
//======================================================================================
MyPlayer::MyPlayer(Uint32 id, const sf::Texture &image, const sf::Font &font, const sf::Vector2f& position, const sf::String& name)
	:Player(id)
{
	setRadius(NEW_PLAYER);
	setCenter(position + Vector2f{ NEW_PLAYER ,NEW_PLAYER });
	setPosition(position);
	Circle::setTexture(&image);
	editText(font, name);
}
//--------------------------------------------------------------------------------------
MyPlayer::MyPlayer()
{
	setOutlineThickness(4);
	setOutlineColor(sf::Color::Black);
	setRadius(NEW_PLAYER);
	setCenter({ NEW_PLAYER ,NEW_PLAYER });

	m_score = unsigned(getRadius());
}
//======================================================================================
OtherPlayers::OtherPlayers(Uint32 id, const sf::Texture &image, const sf::Font &font, float radius, sf::Vector2f position, const sf::String &name)
	:Player(id)
{
	setRadius(radius);
	setPosition(position);
	setCenter(position + Vector2f{ radius, radius });
	setTexture(&image);

	m_score = unsigned(getRadius());
	editText(font, name);
}
//======================================================================================
Food::Food(Uint32 id, sf::Vector2f position, const sf::Texture& t) :FoodAndBomb(id, position)
{
	setRadius(FOOD_RADIUS);
	setCenter(position);
	setOrigin(FOOD_RADIUS, FOOD_RADIUS);
	setFillColor(sf::Color(rand() % 155 + 150, rand() % 155 + 150, rand() % 155 + 150));//?????????????????
	setTexture(&t);
}
//======================================================================================
Bomb::Bomb(Uint32 id, sf::Vector2f position, const sf::Texture& tex) :FoodAndBomb(id, position)
{
	setRadius(BOMB_RADIUS);
	setCenter(position);
	setOrigin(BOMB_RADIUS, BOMB_RADIUS);
	setTexture(&tex);
}


//====================================================================================
//===========================        FUNCTION        =================================
//====================================================================================
void Player::newRadius(Circle *c)
{
	if (dynamic_cast<Food*>(c) || dynamic_cast<Player*>(c))
	{
		setRadius(getRadius() + c->getRadius() / 10);
		setScore(unsigned(getRadius()));
		
		if (dynamic_cast<MyPlayer*>(this))
			Sound::instance().play(FOOD);
	}
	else if (dynamic_cast<Bomb*>(c))
	{
		setRadius(getRadius() / 2);
		setScore(Uint32(getScore() / 2));
		
		if (dynamic_cast<MyPlayer*>(this))
			Sound::instance().play(BOMB);
	}

	m_name.setCharacterSize(unsigned(getRadius() / 2));
}
//-----------------------------------------------------
void Player::move(float x, float y)
{
	sf::CircleShape::move(x, y);
	setCenter(getPosition() + Vector2f{ getRadius(), getRadius() });
	m_name.setOrigin(m_name.getGlobalBounds().width / 2, m_name.getGlobalBounds().height / 2);
	m_name.setPosition(getCenter());
}
//-----------------------------------------------------
void Player::setPosition(const Vector2f &position)
{
	sf::CircleShape::setPosition(position);
	m_name.setPosition(getCenter());
}
//-----------------------------------------------------
float distance(const sf::Vector2f &p1, const sf::Vector2f &p2)
{
	float temp = pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
	return sqrt(temp);
}
//-----------------------------------------------------
bool Player::circlesCollide(const Circle* p) const
{
	return distance(getCenter(), p->getCenter()) <= getRadius() + p->getRadius();
}
//-----------------------------------------------------
bool MyPlayer::legalMove(float speed)
{
	sf::Vector2f moveTo{ 0,0 };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && getCenter().y - getRadius() - speed*MOVE >= 0)
		moveTo += { 0, -speed*MOVE };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && getCenter().y + getRadius() + speed*MOVE <= BOARD_SIZE.y)
		moveTo += { 0, speed*MOVE };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getCenter().x - getRadius() - speed*MOVE >= 0)
		moveTo += { -speed*MOVE, 0 };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getCenter().x + getRadius() + speed*MOVE <= BOARD_SIZE.x)
		moveTo += { speed*MOVE, 0 };

	move(moveTo.x, moveTo.y);

	return moveTo != sf::Vector2f{ 0,0 };
}
//--------------------------------------------------------------------------------------
void Player::editText(const sf::Font &font, const sf::String name)
{
	m_name.setFont(font);
	m_name.setCharacterSize(unsigned(getRadius() / 2));
	m_name.setString(name);
	m_name.setFillColor(sf::Color::White);
	m_name.setOutlineColor(sf::Color::Black);
	m_name.setOutlineThickness(2);
	m_name.setStyle(sf::Text::Bold);
	m_name.setOrigin(m_name.getGlobalBounds().width / 2, m_name.getGlobalBounds().height / 2);
	m_name.setPosition(getCenter());
}

