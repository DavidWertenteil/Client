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
	setFillColor(sf::Color::Cyan);//?????????????????
}
//======================================================================================
Bomb::Bomb(Uint32 id, sf::Vector2f position) :FoodAndBomb(id, position)
{
	setRadius(BOMB_RADIUS);
	setCenter(position);
	setOrigin(BOMB_RADIUS, BOMB_RADIUS);
	setFillColor(sf::Color::Yellow);//?????????????????
}



//====================================================================================
//===========================      CONSTRACTORS      =================================
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
//bool Player::collision(std::vector<Uint32> &deleted, Maps &objectsOnBoard, std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> players)
//{
//	checkFoodAndBomb(deleted, objectsOnBoard);
//	return checkPlayers(players);
//}
////------------------------------------------------------
//void Player::checkFoodAndBomb(std::vector<Uint32> &deleted, Maps &objectsOnBoard)
//{
//	std::set<Uint32> check = objectsOnBoard.colliding(getCenter(),getRadius());
//
//	for (auto it : check) //מחיקה של אוכל ופצצות והוספה לוקטור
//		if (distance(getCenter(), objectsOnBoard[it]->getCenter()) <= getRadius() + objectsOnBoard[it]->getRadius())
//		{
//			std::cout << "my position: " << getPosition().x << " " <<getPosition().y <<
//				" my center: " << getCenter().x << " " << getCenter().y <<
//				"my radius: " << getRadius() << '\n' <<
//				"him position: " << objectsOnBoard[it]->getPosition().x << " " << objectsOnBoard[it]->getPosition().y <<
//				" him center: " << objectsOnBoard[it]->getCenter().x << " " << objectsOnBoard[it]->getCenter().y <<
//				"him radius: " << objectsOnBoard[it]->getRadius() << '\n';
//			deleted.push_back(it);
//			newRadius(objectsOnBoard[it].get());
//			objectsOnBoard.eraseFromData(it);
//		}
//}
////---------------------------------------------
//bool Player::chec
//kPlayers(std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> players)
//{
//	std::vector<Uint32> del;
//	for (auto &player : players)
//		if (distance(getCenter(), player.second->getCenter()) <= getRadius() + player.second->getRadius())
//			if (getRadius() > player.second->getRadius())
//			{
//				setScore(Uint32(player.second->getRadius()));
//				newRadius(player.second.get());
//				del.push_back(player.first);
//			}
//			else
//				return false;
//
//	for (auto p : del)
//		players.erase(p);
//
//	return true;
//}
//
//
//
//


float distance(const sf::Vector2f &p1, const sf::Vector2f &p2)
{
	float temp = std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
	return std::sqrt(temp);
}