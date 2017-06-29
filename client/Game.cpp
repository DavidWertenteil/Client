#pragma once
#include "Game.h"
#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
#define PORT 5555

//====================================================================================
//================================  Score  c-tor =====================================
//====================================================================================
Score::Score() :sf::Text("score: " + std::to_string(NEW_PLAYER), Fonts::instance()[SCORE], 40) {
	setPosition({ 10, float(sf::VideoMode::getDesktopMode().height) - getGlobalBounds().height - 10 });
	setFillColor(sf::Color(105, 105, 105));
}
//====================================================================================
//================================ CONSTRUCTOR =======================================
//====================================================================================
Game::Game(const std::string& ip, Uint32 image_id, sf::View& view, const sf::String &name)
	:m_me(std::make_unique<MyPlayer>()),
	m_background(Images::instance().getImage(BACKGROUND)),
	m_view(view),
	m_score(),
	m_minimap(sf::FloatRect{ 0, 0, float(BOARD_SIZE.x), float(BOARD_SIZE.y) }),
	m_minimapBackground(BOARD_SIZE)
{

	while (true)
	{
		try
		{
			connectToServer(ip);
		}
		catch (std::exception &ex)
		{
			system("cls");
			std::cout << ex.what() << std::endl;
			continue; //not break the loop
		}

		break;
	}

	sf::Packet packet;
	packet << image_id << name; //send my pic and name to the server
	if (m_socket.send(packet) != sf::TcpSocket::Done)
		std::cout << "no sending image\n";

	receive();//receive the board
	m_me->editText(Fonts::instance()[SETTINGS], name);

	m_minimap.setViewport(sf::FloatRect{ 0,0,0.15f, 0.2f });
	m_minimapBackground.setFillColor(sf::Color(105, 105, 105, 100));
}
//--------------------------------------------------------------------------
void Game::connectToServer(const std::string& ip)
{
	if (m_socket.connect(ip, PORT) != sf::TcpSocket::Done)
		throw std::exception{ "no connecting, please wait" };
}
//--------------------------------------------------------------------------
void Game::receive()
{
	sf::Packet packet;
	std::pair <Uint32, sf::Vector2f> temp;
	Uint32 image;
	sf::String name;
	float radius;

	m_socket.setBlocking(true);//**********************************
	Sleep(100);//*********************************

	auto status = m_socket.receive(packet);

	if (status == sf::TcpSocket::Done)
	{
		while (!packet.endOfPacket())//receive the board
		{
			packet >> temp;

			if (temp.first >= FOOD_LOWER && temp.first <= BOMBS_UPPER)
				m_objectsOnBoard.insert(temp);

			else if (temp.first >= PLAYER_LOWER && temp.first <= PLAYER_UPPER)//
			{
				packet >> radius >> image >> name;
				m_players.emplace(temp.first, std::make_unique<OtherPlayers>(temp.first, Images::instance()[image], Fonts::instance()[SETTINGS], radius, temp.second, name));
			}
		}
	}

	m_players.erase(temp.first); //delete my player from the other player
    
	//update my player
	m_me->setId(temp.first);
	m_me->setTexture(Images::instance()[image]);
	m_me->setPosition(temp.second);
	m_me->setCenter(temp.second + Vector2f{ NEW_PLAYER,NEW_PLAYER });

	//std::cout << std::string(name) << '\n';******************************
}

//====================================================================================
//================================     PLAY     ======================================
//====================================================================================
//the main loop of the game
unsigned Game::play(sf::RenderWindow &w)
{
	m_socket.setBlocking(false);

	sf::Packet packet;
	sf::Event event;
	float lastMove = 0;

	while (m_me->getLive())
	{
		w.pollEvent(event);
		auto speed = TimeClass::instance().RestartClock();
		lastMove += TimeClass::instance().getTime();
		
		//player move
		if (m_receive) // if the player receive his last move
			if (event.type == sf::Event::EventType::KeyPressed)
				updateMove(speed, lastMove);

		//receive changes from the server
		receiveChanges();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return m_me->getScore();

		if (TimeClass::instance().getTime() - lastMove > 0.05)
			m_receive = true;


		m_score.setScore(m_me->getScore());
		display(w);
	}

	return m_me->getScore();
}

//====================================================================================
//===========================      UPDATE MOVE       =================================
//====================================================================================
// my move
void Game::updateMove(float speed, float &lastMove)
{
	sf::Packet packet;
	packet.clear();

	if (m_me->legalMove(speed))
	{
		std::vector<Uint32> deleted; //all the object that delete in this move
		m_me->collision(deleted, m_objectsOnBoard, m_players, m_me.get(), m_lastDead);

		if (!m_me->getLive())
			deleted.push_back(m_me->getId()); // if i dead, update the server

		packet << m_me->getId() << m_me->getRadius() << m_me->getPosition() << deleted;

		if (m_socket.send(packet) != sf::TcpSocket::Done)
			std::cout << "no sending data\n";

		if (!m_me->getLive())
			Sleep(100);

		m_receive = false;
		lastMove = 0; //the time from the last move is 0
	}
}

//====================================================================================
//===========================      RECEIVE DATA      =================================
//====================================================================================
void Game::receiveChanges()
{
	sf::Packet packet;

	if (m_socket.receive(packet) == sf::TcpSocket::Partial)
		m_receive = true;

	while (!packet.endOfPacket())
	{
		std::pair<Uint32, sf::Vector2f> temp;
		if (!(packet >> temp))
			continue;
		std::vector<Uint32> del;

		if (temp.first >= FOOD_LOWER && temp.first <= BOMBS_UPPER) //new foods or bombs
			m_objectsOnBoard.insert(temp);

		else if (temp.first >= PLAYER_LOWER && temp.first <= PLAYER_UPPER)// player id
		{
			if (temp.first == m_me->getId())// if it's my player
				m_receive = true;

			else if (m_players.find(temp.first) != m_players.end())// if it's player's move
			{
				m_players[temp.first]->setPosition(temp.second);
				m_players[temp.first]->setCenter();
				m_players[temp.first]->collision(del, m_objectsOnBoard, m_players, m_me.get(), m_lastDead);
			}

			else //if it's a new player
			//try
			{
				if (m_lastDead != temp.first)
					addPlayer(temp, packet);
			}
			//catch (...)
			{

			}
		}
	}

	deleteDeadPlayer(m_players);
}
//------------------------------------------------------------------------------------
void Game::addPlayer(const std::pair<Uint32, sf::Vector2f> &temp, sf::Packet &packet)
{
	Uint32 image;
	sf::String name;
	packet >> image >> name;
	m_players.emplace(temp.first, std::make_unique<OtherPlayers>(temp.first, Images::instance()[image], Fonts::instance()[SETTINGS], NEW_PLAYER, temp.second, name));
}
//------------------------------------------------------------------------------------
void deleteDeadPlayer(std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players)
{
	for (auto it = players.begin(); it != players.end();)
		(it->second->getLive()) ? it++ : it = players.erase(it);
}
//====================================================================================
//===========================          PRINT         =================================
//====================================================================================
sf::Vector2f Game::setView(sf::RenderWindow &w) const
{
	sf::Vector2f pos{ float(SCREEN_WIDTH) / 2 , float(SCREEN_HEIGHT) / 2 };

	if (m_me->getCenter().x > SCREEN_WIDTH / 2)
		if (BOARD_SIZE.x - m_me->getCenter().x < SCREEN_WIDTH / 2)
			pos.x = BOARD_SIZE.x - SCREEN_WIDTH / 2;
		else
			pos.x = m_me->getCenter().x;

	if (m_me->getCenter().y > SCREEN_HEIGHT / 2)
		if (BOARD_SIZE.y - m_me->getCenter().y < SCREEN_HEIGHT / 2)
			pos.y = BOARD_SIZE.y - SCREEN_HEIGHT / 2;
		else
			pos.y = m_me->getCenter().y;

	return pos;
}
//--------------------------------------------------------------------------
void Game::draw(sf::RenderWindow &w) const 
{
	for (auto &x : m_objectsOnBoard)
		w.draw(*x.second.get());

	for (auto &x : m_players)
	{
		w.draw(*x.second.get());
		w.draw(x.second->getName());
	}

	w.draw(*m_me.get());//******************** why the name in another function?
}
//--------------------------------------------------------------------------
void Game::display(sf::RenderWindow &w)
{
	w.clear();
	
	//-------------------- background ---------------------
	auto pos = setView(w);
	m_view.setCenter(pos);
	w.setView(m_view);
	w.draw(m_background);
	draw(w);
	w.draw(m_me->getName());

	w.setView(m_minimap);
	draw(w);
	w.draw(m_minimapBackground);
	
	//---------------------- score ------------------------
	m_view.setCenter(float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2));
	w.setView(m_view);
	w.draw(m_score);
	
	//--------------------- list score --------------------
	m_scoreList.display(w, m_players, m_me);

	w.display();
}


//*************************************************************************************
//****************************    PLAYER FUNCTION   ***********************************
//*************************************************************************************
void Player::collision(std::vector<Uint32> &deleted, Maps &objectsOnBoard, std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players, Player *me, Uint32 &lastDead)
{
	checkPlayers(deleted, players, me, lastDead);
	checkFoodAndBomb(deleted, objectsOnBoard);
}
//--------------------------------------------------------------------------
void Player::checkPlayers(std::vector<Uint32> &deleted, std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players, Player *me, Uint32 &lastDead)
{
	for (auto &player : players)
	{
		if (player.second->getId() == getId())
			continue;

		if (circlesCollide(player.second.get()))
			if (getRadius() > player.second->getRadius()) //if two circles collided and the other player dead
			{
				newRadius(player.second.get());
				player.second->setLive(false);
				deleted.push_back(player.first);

				lastDead = player.second->getId(); //***************************************
			}
			else
				m_live = false; //if the player thar called this function is dead
	}


	if (dynamic_cast<OtherPlayers*>(this)) //check colliding with my player
	{
		if (circlesCollide(me)) 
			if (getRadius() > me->getRadius()) //if my player is dead
			{
				me->setLive(false);
				newRadius(me);
			}
			else
			{
				m_live = false; //if the player thar called this function is dead
				me->newRadius(this);

				lastDead = getId();//**************************************************
			}
	}

	deleteDeadPlayer(players);
}
//--------------------------------------------------------------------------
void Player::checkFoodAndBomb(std::vector<Uint32> &deleted, Maps &objectsOnBoard)
{
	//***************************************************************************
	std::set<Uint32> check = objectsOnBoard.colliding(getCenter(), getRadius());
	//***************************************************************************

	for (auto it : check) 
		if (circlesCollide(objectsOnBoard[it].get()))
		{
			newRadius(objectsOnBoard[it].get());
			objectsOnBoard.eraseFromData(it);
			deleted.push_back(it);
		}

	if (getRadius() < NEW_PLAYER)
		m_live = false;
}