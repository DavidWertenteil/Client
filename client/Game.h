#pragma once
#include "Utilities.h"
#include <exception>
#include <map>
#include "ScoreList.h"


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
	Game(const std::string&, Uint32, sf::View&, const sf::String &);

	void receive();
	unsigned play(sf::RenderWindow &w);

	void draw(sf::RenderWindow &w)const;
	void display(sf::RenderWindow &w);
	sf::Vector2f setView(sf::RenderWindow &w)const;

	void deleteDeadPlayer(std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players);
	void eraseFromData(Uint32 it) { m_objectsOnBoard.eraseFromData(it); }



	Maps getObjectsOnBoard()const { return m_objectsOnBoard; }
	std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> getPlayers() const { return m_players; }
	MyPlayer* getMe() const { return m_me.get(); }
	void setLastDead(Uint32 x) { m_lastDead = x; }

private:
	void connectToServer(const std::string&);
	void updateMove(float, float &);
	void receiveChanges();
	void addPlayer(const std::pair<Uint32, sf::Vector2f> &temp, sf::Packet &packet);
	//----------------------------

	Maps m_objectsOnBoard;
	std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> m_players;
	sf::Sprite m_background;
	std::unique_ptr<MyPlayer> m_me;
	Score m_score;
	sf::TcpSocket m_socket;
	sf::View& m_view;
	sf::View m_minimap;
	sf::RectangleShape m_minimapBackground;
	ScoreList m_scoreList;
	bool m_receive = true;
	Uint32 m_lastDead = 0;
};

