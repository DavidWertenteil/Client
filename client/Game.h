#pragma once
#include "Utilities.h"
#include <map>

//--------------------------------------------------------------------------
class Score :public sf::Text {
public:
	Score();
	~Score() {}
	void setScore(unsigned s) { setString("score: " + std::to_string(s)); }
};
//============================================================================================
class Game
{
public:
	Game(Uint32 image, sf::View&, const sf::String &name = "no name");
	void receive();
	unsigned play(sf::RenderWindow &w);

	void draw(sf::RenderWindow &w)const;
	void display(sf::RenderWindow &w);
	sf::Vector2f setView(sf::RenderWindow &w)const;

private:
	void updateMove(float);
	void receiveChanges();
	void addPlayer(const std::pair<Uint32, sf::Vector2f> &temp, sf::Packet &packet);
	//===========================
	Maps m_objectsOnBoard;
	std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> m_players;
	sf::Sprite m_background;
	std::unique_ptr<MyPlayer> m_me;
	Score m_score;
	sf::TcpSocket m_socket;
	sf::View& m_view;
	sf::View m_minimap;
	sf::RectangleShape m_minimapBackground;

	bool m_receive = true;
};

void deleteDeadPlayer(std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players);
