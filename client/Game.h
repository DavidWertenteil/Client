#pragma once
#include "Utilities.h"
#include <map>



class Game
{
public:
	Game(const Images &images, const Fonts &fonts, Uint32 image,sf::View&, const sf::String &name = "no name");
	void receive(const Images &images, const Fonts &fonts);
	unsigned play(sf::RenderWindow &w, const Images &images, const Fonts &fonts);
	
	void draw(sf::RenderWindow &w) const;
	void setView(sf::RenderWindow &w) const;

private:
	void updateMove(float);
	void receiveChanges(const sf::Event &event, const Images &images, const Fonts &fonts);
	void addPlayer(const std::pair<Uint32, sf::Vector2f> &temp, sf::Packet &packet, const Images &images, const Fonts &fonts);
	//===========================
	Maps m_objectsOnBoard;
	std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> m_players;
	sf::Sprite m_background;
	std::unique_ptr<MyPlayer> m_me;
	//m_miniMap;
	//Score m_score;
	sf::TcpSocket m_socket;
	sf::View& m_view; 

	bool m_receive = true;
};
//--------------------------------------------------------------------------
	void deleteDeadPlayer(std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players);
