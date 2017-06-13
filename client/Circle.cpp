#pragma once
#include "Circle.h"


//====================================================================================
//===========================      CONSTRACTORS      =================================
//====================================================================================
MyPlayer::MyPlayer(Uint32 id, const sf::Texture &image, sf::Vector2f position)
	:Player(id)
{
	setRadius(NEW_PLAYER);
	setCenter(position + Vector2f{ NEW_PLAYER ,NEW_PLAYER });
	setPosition(position);
	Circle::setTexture(&image);

}
//--------------------------------------------------------------------------------------
MyPlayer::MyPlayer()
{
	setRadius(NEW_PLAYER);
	setCenter({ NEW_PLAYER ,NEW_PLAYER });
}
//======================================================================================
OtherPlayers::OtherPlayers(Uint32 id, const sf::Texture &image, float radius, sf::Vector2f position)
	:Player(id)
{
	setRadius(radius);
	setPosition(position);
	setCenter(position + Vector2f{radius, radius});
	setTexture(&image);
}
//======================================================================================
Food::Food(Uint32 id, sf::Vector2f position) :FoodAndBomb(id, position)
{
	setRadius(FOOD_RADIUS);
	setCenter(position);
	setOrigin(FOOD_RADIUS, FOOD_RADIUS);
	setFillColor(sf::Color(rand()%155+150, rand() % 155 + 150, rand() % 155 + 150));//?????????????????
	setOutlineColor(sf::Color(getFillColor().r, getFillColor().g, getFillColor().b,100) );
	setOutlineThickness(4);
}
//======================================================================================
Bomb::Bomb(Uint32 id, sf::Vector2f position) :FoodAndBomb(id, position)
{
	setRadius(BOMB_RADIUS);
	setCenter(position);
	setOrigin(BOMB_RADIUS, BOMB_RADIUS);
	setFillColor(sf::Color::Red);//?????????????????
}

//====================================================================================
//===========================        FUNCTION        =================================
//====================================================================================
void Player::newRadius(const Circle *c)
{
	setRadius(getRadius() + c->getRadius() / 10);
}
//-----------------------------------------------------
void Player::move(float x, float y)
{
	sf::CircleShape::move(x, y);
	setCenter(getPosition() + Vector2f{ getRadius(), getRadius() });
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

