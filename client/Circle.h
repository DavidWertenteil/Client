#pragma once
#include <SFML\Graphics.hpp>
#include<SFML\Network.hpp>
#include <memory>
#include <set>
//#include "Utilities.h"
#include <iostream>
#include <unordered_map>

class Maps;

#ifdef _DEBUG
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#elif defined(NDEBUG)
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-network.lib")
#else
#error "Unrecognized configuration!"
#endif

const float FOOD_RADIUS = 10;
const float BOMB_RADIUS = 30;
const float NEW_PLAYER = 60;

using sf::Uint32;
using sf::CircleShape;
using sf::Vector2f;


class Circle :public CircleShape
{
public:
	Circle() = default;
	Circle(Uint32 id) :m_id(id) {}

	Uint32 virtual getId() const { return m_id; }
	Vector2f getCenter() const { return m_center; }
	void setCenter(Vector2f center) { m_center = center; }
	

protected:
	Uint32 m_id;
	Vector2f m_center;

};
//-------------------------------------
class Player :public Circle
{
public:
	Player() = default;
	Player(Uint32 id) :Circle(id) {};

	//bool collision(std::vector<Uint32> &deleted, Maps &objectsOnBoard, std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> players);
	//bool checkPlayers(std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> );
	//void checkFoodAndBomb(std::vector<Uint32> &, Maps &);

	void newRadius(const Circle *c);
	void move(float x, float y);

	void setScore(Uint32 radius) { m_score += unsigned(radius); }
	unsigned getScore() const { return m_score; }

protected:
	unsigned m_score = 0;
};
//-------------------------------------
class MyPlayer :public Player
{
public:
	MyPlayer();
	MyPlayer(Uint32 id, const sf::Texture &image, sf::Vector2f position = { 0.f,0.f });

	void setId(Uint32 id) { m_id = id; }
	void setTexture(const sf::Texture &image) { CircleShape::setTexture(&image); }
	//bool collision() const override { return true; }

};
//-------------------------------------
class OtherPlayers :public Player
{
public:
	OtherPlayers(Uint32 id, const sf::Texture &image, float radius, sf::Vector2f position);
};
//-------------------------------------
class FoodAndBomb :public Circle
{
public:
	FoodAndBomb(Uint32 id, sf::Vector2f place) :Circle(id) { setPosition(place); }
	FoodAndBomb(std::pair<Uint32, sf::Vector2f> temp) :FoodAndBomb(temp.first, temp.second) {}
};
//-------------------------------------
class Food :public FoodAndBomb
{
public:
	Food(Uint32 id, sf::Vector2f place);
	Food(std::pair<Uint32, sf::Vector2f> temp) :Food(temp.first, temp.second) {}
};
//-------------------------------------
class Bomb :public FoodAndBomb
{
public:
	Bomb(Uint32 id, sf::Vector2f place);
	Bomb(std::pair<Uint32, sf::Vector2f> temp) :Bomb(temp.first, temp.second) {}
};