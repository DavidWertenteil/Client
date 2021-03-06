#pragma once
#include "Utilities.h"
#include <iostream>

//==========================================================================
//=================================MAPS=====================================
//==========================================================================
void Maps::insert(const std::pair<Uint32, sf::Vector2f> &temp)
{
	if (temp.first >= FOOD_LOWER && temp.first <= FOOD_UPPER)
		emplace(temp.first, std::make_unique<Food>(temp, Images::instance()[FOOD_PIC]));
	else
		emplace(temp.first, std::make_unique<Bomb>(temp, Images::instance()[BOMB_PIC]));

	m_x.emplace(temp.second.x, temp.first);
	m_y.emplace(temp.second.y, temp.first);

}

//---------------------------------------------------------------------------
std::set<Uint32> Maps::colliding(const Vector2f& ver, const float radius)
{
	std::set<Uint32> intersection;

	for (auto x = begin(); x != end(); x++)
		intersection.insert(x->first);
	
	return intersection;
}
//---------------------------------------------------------------------------
//erase from x and y map
template<typename T>
void eraseFromMap(T& map, sf::Uint32 id, float bound) {

	for (auto it = map.find(bound); it != map.end() && it != map.upper_bound(bound + 1.f); ++it)
		if (it->second == id) {
			map.erase(it->first);
			return;
		}
}
//---------------------------------------------------------------------------
void Maps::eraseFromData(sf::Uint32 id) {

	//erase from x map
	eraseFromMap(m_x, id, find(id)->second->getPosition().x);
	//erase from y map
	eraseFromMap(m_y, id, find(id)->second->getPosition().y);
	//erase from unorded map
	erase(id);
}

//==========================================================================
//================================PACKET====================================
//==========================================================================
sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& vertex)
{
	return packet >> vertex.x >> vertex.y;
}
//------------------------------------------------------------------------------
sf::Packet& operator >> (sf::Packet& packet, std::pair<Uint32, sf::Vector2f>& pair)
{
	return packet >> pair.first >> pair.second;
}
//------------------------------------------------------------------------------
sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f &vertex)
{
	return packet << vertex.x << vertex.y;
}
//------------------------------------------------------------------------------
sf::Packet& operator << (sf::Packet& packet, const std::vector<Uint32>& deleted)
{
	for (auto x : deleted)
		packet << x;
	return packet;
}

