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
class Maps :public std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>> {
public:
	Maps() {}
	~Maps() {}

	//using std::unordered_map<Uint32, std::unique_ptr<FoodAndBomb>>::emplace;

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


////                 שליחה של כל הלוח בתחילת המשחק
////==================================================================================================================
//sf::Packet& operator << (sf::Packet& packet, const std::unique_ptr<Circle>& c)
//{
//	packet << c->getId() << c->getVertex().x << c->getVertex().y;
//	if (auto it = dynamic_cast<Player*>(c.get())) //if its a player 
//		packet << it->getRadius() << it->getImage();
//
//	return packet;
//}
////==================================================================================================================
//sf::Packet& operator << (sf::Packet& packet, const std::pair<const sf::Uint32, std::unique_ptr<Circle>>& pair)
//{
//	return packet << pair.second;
//}
////=================================================================================================================
///*send the board*/
//void sendMap() {
//	for (auto it = board.begin(); it != board.end(); ++it) {
//		packet << it->second;
//	}
//}
////==================================================================================================================
//// class Board :public std::unordered_map<sf::Uint32, std::unique_ptr<Circle>> המבנה נתונים של הלוח
//
//
////                          שליחת נתונית באמצע משחק לכל שחקן
////==============================================================================================================
//sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f& v)
//{
//	return packet << v.x << v.y;
//}
////==================================================================================================================
//sf::Packet& operator << (sf::Packet& packet, const sendPack& p)
//{
//	return packet << p._ver << p._id;
//}
////==================================================================================================================
//sf::Packet& operator << (sf::Packet& packet, std::queue<sendPack>& que)
//{
//	while (!que.empty()) {
//		packet << que.front();
//		que.pop();
//	}
//	return packet;
//}
////====================================================================================================================
//
////                                          קליטת נתונים מכל שחקן באמצע משחק
////==============================================================================================================
//sf::Packet& operator >> (sf::Packet& packet, std::vector<sf::Uint32>& vec)
//{
//	for (auto v : vec)
//		packet >> v;
//	return packet;
//}
////==============================================================================================================
//sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& v)
//{
//	return packet >> v.x >> v.y;
//}
////==============================================================================================================
//sf::Packet& operator >> (sf::Packet& packet, recPack& p)
//{
//	return packet >> p._ver >> p._id >> p._vec >> p._image;
//}
////==============================================================================================================
template<typename T>
void eraseFromMap(T& map, sf::Uint32 id, float bound);