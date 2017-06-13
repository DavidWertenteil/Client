#pragma once
#include "Game.h"
#include <iostream>
#include <vector>

Game::Game(const Images &images, Uint32 image_id):
	m_background(images[int(BACKGROUND)])
{
	if (m_socket.connect(sf::IpAddress::LocalHost, 5555) != sf::TcpSocket::Done)
		//if (m_socket.connect("10.2.15.207", 5555) != sf::TcpSocket::Done)
		std::cout << "no connecting\n";

	sf::Packet packet;
	packet << image_id; //שליחה לשרת של התמונה שלי
	if (m_socket.send(packet) != sf::TcpSocket::Done)
		std::cout << "no sending image\n";

	receive(images);//קליטת מידע מהשרת
}
//--------------------------------------------------------------------------
void Game::receive(const Images &images)
{
	sf::Packet packet;
	std::pair <Uint32, sf::Vector2f> temp;
	Uint32 image;
	float radius;

	if (m_socket.receive(packet) == sf::TcpSocket::Done)
		while (!packet.endOfPacket())//קליטה של כל הדברים שעל הלוח
		{
			packet >> temp;

			if (temp.first >= 1000 && temp.first <= 10000)
				m_objectsOnBoard.insert(temp);

			else if (temp.first >= 200 && temp.first <= 300)//???????????????????????
			{
				packet >> radius >> image;
				m_players.emplace(temp.first, std::make_unique<OtherPlayers>(temp.first, images[image], radius, temp.second));
			}
		}

	m_players.erase(temp.first); //הורדת העיגול שלי מהשחקנים האחרים

	m_me.setId(temp.first);//עדכון העיגול שלי
	m_me.setTexture(images[image]);
	m_me.setPosition(temp.second);
	m_me.setCenter(temp.second + Vector2f{ NEW_PLAYER,NEW_PLAYER });
}
//--------------------------------------------------------------------------
unsigned Game::play(sf::RenderWindow &w, const Images &images)
{
	m_socket.setBlocking(false);

	sf::Packet packet;
	sf::Event event;
	while (true)
	{
		//		bool canMove;
		w.pollEvent(event);

		//תזוזה של השחקן
		if (event.type == sf::Event::EventType::KeyPressed)
			if (!updateMove(event))
				return m_me.getScore();

		//קבלת מידע מהשרת
		receiveChanges(event, images);

		w.draw(m_background);
		draw(w);
	}


	return 0;
}




//====================================================================================
//===========================      UPDATE MOVE       =================================
//====================================================================================
//מחזיר שקר אם מתתי
bool Game::updateMove(const sf::Event &event)
{
	sf::Packet packet;

	if (legalMove(event))
	{
		std::vector<Uint32> deleted;
		//if (!m_me.collision(deleted, m_objectsOnBoard, m_players)) //אם מתתי
			//return false;
		if (!collision(deleted, m_me))
			return false;

		packet.clear();
		packet << m_me.getId() << m_me.getRadius() << m_me.getPosition() << deleted;


		if (m_socket.send(packet) != sf::TcpSocket::Done)
			std::cout << "no sending data\n";
	}

	return true;
}
//--------------------------------------------------------------------------
bool Game::legalMove(const sf::Event &event)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		if (m_me.getCenter().y - m_me.getRadius() - MOVE < 0)
			return false;
		else
			m_me.move(0, -MOVE);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		if (m_me.getCenter().y + m_me.getRadius() + MOVE > BOARD_SIZE.y)
			return false;
		else
			m_me.move(0, MOVE);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		if (m_me.getCenter().x - m_me.getRadius() - MOVE < 0)
			return false;
		else
			m_me.move(-MOVE, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		if (m_me.getCenter().x + m_me.getRadius() + MOVE > BOARD_SIZE.x)
			return false;
		else
			m_me.move(MOVE, 0);


	return true;
}



//*************************************************************************************
//*************************************************************************************
//*************************************************************************************
bool Game::collision(std::vector<Uint32> &deleted, Player &p)
{
	checkFoodAndBomb(deleted, p);
	return checkPlayers(p);
}
//--------------------------------------------------------------------------
void Game::checkFoodAndBomb(std::vector<Uint32> &deleted, Player &p)
{
	std::set<Uint32> check = m_objectsOnBoard.colliding(p.getCenter(), p.getRadius());

	for (auto it : check) //מחיקה של אוכל ופצצות והוספה לוקטור
		if (distance(p.getCenter(), m_objectsOnBoard[it]->getCenter()) <= p.getRadius() + m_objectsOnBoard[it]->getRadius())
		{
			std::cout << "my position: " << p.getPosition().x << " " << p.getPosition().y <<
				" my center: " << p.getCenter().x << " " << p.getCenter().y <<
				"my radius: " << p.getRadius() << '\n' <<
				"him position: " << m_objectsOnBoard[it]->getPosition().x << " " << m_objectsOnBoard[it]->getPosition().y <<
				" him center: " << m_objectsOnBoard[it]->getCenter().x << " " << m_objectsOnBoard[it]->getCenter().y <<
				"him radius: " << m_objectsOnBoard[it]->getRadius() << '\n';
			if (p.getId() == m_me.getId()) // תוסיף לוקטור המחיקה רק אם אני פגעתי בהם
				deleted.push_back(it);
			p.newRadius(m_objectsOnBoard[it].get());
			m_objectsOnBoard.eraseFromData(it);
		}

}
//--------------------------------------------------------------------------
//מחזיר שקר אם השחקן מת
bool Game::checkPlayers(Player &p)
{
	std::vector<Uint32> del;
	for (auto &player : m_players)
	{
		if (player.second->getId() == p.getId())
			continue;
		if (distance(p.getCenter(), player.second->getCenter()) <= p.getRadius() + player.second->getRadius())
			if (p.getRadius() > player.second->getRadius())
			{
				p.setScore(Uint32(player.second->getRadius()));
				p.newRadius(player.second.get());
				//m_players.erase(player.first);
				del.push_back(player.first);
			}
			else
				return false;
	}

	for (auto pl : del)
		m_players.erase(pl);

	return true;
}
//--------------------------------------------------------------------------
float Game::distance(const sf::Vector2f &p1, const sf::Vector2f &p2)
//float distance(const sf::Vector2f &p1, const sf::Vector2f &p2)
{
	float temp = std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
	return std::sqrt(temp);
}
//*************************************************************************************
//*************************************************************************************
//*************************************************************************************
//*************************************************************************************


//====================================================================================
//===========================      RECEIVE DATA      =================================
//====================================================================================
//מחזיר שקר אם מתתי
bool Game::receiveChanges(const sf::Event &event, const Images &images)
{
	sf::Packet packet;
	m_socket.receive(packet);

	while (!packet.endOfPacket())
	{
		std::pair<Uint32, sf::Vector2f> temp;
		packet >> temp;
		std::vector<Uint32> del;
		Uint32 player_delete = 1;

		if (temp.first >= 1000 && temp.first <= 10000) // אוכל או פצצות חדשות
			m_objectsOnBoard.insert(temp);

		else if (temp.first >= 200 && temp.first <= 300)// שחקן
		{
			if (temp.first == m_me.getId())// השחקן שלי
				continue;
			if (m_players.find(temp.first) != m_players.end())// תזוזה של שחקן (שחקן קיים..)י
			{
				m_players[temp.first]->setPosition(temp.second);
				m_players[temp.first]->setCenter(m_players[temp.first]->getPosition() + Vector2f{ m_players[temp.first]->getRadius(),m_players[temp.first]->getRadius() });
				if (!collision(del, *m_players[temp.first].get()))
					m_players.erase(temp.first);
			}
			else // שחקן חדש
			{
				Uint32 image;
				packet >> image;
				image %= 11;//************************************************************
				image++;//*************************************************************
				m_players.emplace(temp.first, std::make_unique<OtherPlayers>(temp.first, images[image], NEW_PLAYER, temp.second));
			}
		}
	}

	return true;
}


//====================================================================================
//===========================          PRINT         =================================
//====================================================================================
void Game::setView(sf::RenderWindow &w) const
{
	sf::View view;
	view.reset(sf::FloatRect{ 0,0,float(SCREEN_WIDTH),float(SCREEN_HEIGHT) });
	sf::Vector2f pos{ float(SCREEN_WIDTH) / 2 , float(SCREEN_HEIGHT) / 2 };

	if (m_me.getCenter().x > SCREEN_WIDTH / 2)
		if (BOARD_SIZE.x - m_me.getCenter().x < SCREEN_WIDTH / 2)
			pos.x = BOARD_SIZE.x - SCREEN_WIDTH / 2;
		else
			pos.x = m_me.getCenter().x;

	if (m_me.getCenter().y > SCREEN_HEIGHT / 2)
		if (BOARD_SIZE.y - m_me.getCenter().y < SCREEN_HEIGHT / 2)
			pos.y = BOARD_SIZE.y - SCREEN_HEIGHT / 2;
		else
			pos.y = m_me.getCenter().y;

	view.setCenter(pos);
	w.setView(view);
}
//--------------------------------------------------------------------------
void Game::draw(sf::RenderWindow &w) const
{
	setView(w);

	w.clear();

	for (auto &x : m_objectsOnBoard)
		w.draw(*x.second.get());
	for (auto &x : m_players)
		w.draw(*x.second.get());
	w.draw(m_me);

	w.display();

}


