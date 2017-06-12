#pragma once
#include <SFML\Graphics.hpp>
#include<SFML\Network.hpp>
#include <memory>
#include <unordered_map>
#include <set>
#include "Circle.h"

//==========================================================================
//=================================MAPS=====================================
//==========================================================================
//================================ Coordinates class =======================
/***************************************************************************
this class is for all the food and bombs in the game:
it add's food and bombs when needed and removes them when a ball
crashes into them.
Both multimaps are for finding the colliding food and bombs with the player'

******************************************************************************************/
class Maps :private std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>> {
public:
	Maps() {}
	~Maps() {}

	using std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>>::emplace;
	using std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>>::erase;
	using std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>>::operator[];
	using std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>>::begin;
	using std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>>::end;

	void insert(const std::pair<Uint32, sf::Vector2f> &);
	void eraseFromData(sf::Uint32 );
	std::set<Uint32> Maps::colliding(const Vector2f& ver, const float radius);
private:

	std::multimap<float, Uint32> m_x;
	std::multimap<float, Uint32> m_y;
};



//==========================================================================
//================================PACKET====================================
//==========================================================================

sf::Packet& operator >> (sf::Packet& packet, std::pair <Uint32, sf::Vector2f>& pair);
sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& vertex);

sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f &vertex);
sf::Packet& operator << (sf::Packet& packet, const std::vector<Uint32>& deleted);
