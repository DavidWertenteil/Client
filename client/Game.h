#pragma once
#include "Utilities.h"
#include "Images.h"
#include <memory>
#include <map>
#include <unordered_map>

const float MOVE = 2;
const sf::Vector2f BOARD_SIZE{ 3000.f,3000.f };


class Game
{
public:
	Game(const Images &images, Uint32 image);
	void receive(const Images &images);
	unsigned play(sf::RenderWindow &w, const Images &images);
	
	void draw(sf::RenderWindow &w) const;
	void setView(sf::RenderWindow &w) const;
private:
	bool updateMove(const sf::Event &event);
	bool legalMove(const sf::Event &event);
	float distance(const sf::Vector2f &p1, const sf::Vector2f &p2);
	
	bool collision(std::vector<Uint32> &, Player &p);
	bool checkPlayers( Player &p);
	void checkFoodAndBomb(std::vector<Uint32> &, Player &p);

	bool receiveChanges(const sf::Event &event, const Images &images);

	//===========================
	Maps m_objectsOnBoard;
	std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> m_players;
	sf::Sprite m_background;
	MyPlayer m_me;
	//m_miniMap;
	//Score m_score;
	sf::TcpSocket m_socket;
};
//--------------------------------------------------------------------------
