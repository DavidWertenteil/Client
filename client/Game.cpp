#pragma once
#include "Game.h"
#include <iostream>
#include <vector>
#include <Windows.h>

//====================================================================================
//================================  Score  c-tor =====================================
//====================================================================================
Score::Score(const Fonts &fonts) :sf::Text("score: " + std::to_string(NEW_PLAYER), fonts[SCORE], 40) {
	setPosition({ 10, float(sf::VideoMode::getDesktopMode().height) - getGlobalBounds().height - 10 });
	setFillColor(sf::Color(105, 105, 105));
}
//====================================================================================
//================================ CONSTRUCTOR =======================================
//====================================================================================
Game::Game(const Images &images, const Fonts &fonts, Uint32 image_id, sf::View& view, const sf::String &name)
	:m_me(std::make_unique<MyPlayer>()),
	m_background(images.getImage(BACKGROUND)),
	m_view(view),
	m_score(fonts),
	m_minimap(sf::FloatRect{ 0, 0, float(BOARD_SIZE.x), float(BOARD_SIZE.y) }),
	m_minimapBackground(BOARD_SIZE)
{

	if (m_socket.connect(sf::IpAddress::LocalHost, 5555) != sf::TcpSocket::Done)
	//if (m_socket.connect("10.2.16.95", 5555) != sf::TcpSocket::Done)
		std::cout << "no connecting" << std::endl;

	sf::Packet packet;
	packet << image_id << name; //שליחה לשרת של התמונה שלי
	if (m_socket.send(packet) != sf::TcpSocket::Done)
		std::cout << "no sending image\n";

	receive(images, fonts);//קליטת מידע מהשרת
	m_me->editText(fonts[SETTINGS], name);

	m_minimap.setViewport(sf::FloatRect{ 0,0,0.15f, 0.2f });
	m_minimapBackground.setFillColor(sf::Color(105, 105, 105, 150));
}
//=============================================================================================================
//--------------------------------------------------------------------------
void Game::receive(const Images &images, const Fonts &fonts)
{
	sf::Packet packet;
	std::pair <Uint32, sf::Vector2f> temp;
	Uint32 image;
	sf::String name, s = "asd";
	float radius;

	m_socket.setBlocking(true);//**********************************
	Sleep(100);//*********************************

	auto status = m_socket.receive(packet);

	if (status == sf::TcpSocket::Done)
	{
		while (!packet.endOfPacket())//קליטה של כל הדברים שעל הלוח
		{
			packet >> temp;

			if (temp.first >= FOOD_LOWER && temp.first <= BOMBS_UPPER)
				m_objectsOnBoard.insert(temp, images);

			else if (temp.first >= PLAYER_LOWER && temp.first <= PLAYER_UPPER)//
			{
				packet >> radius >> image >> name;
				m_players.emplace(temp.first, std::make_unique<OtherPlayers>(temp.first, images[image], fonts[SETTINGS], radius, temp.second, name));
			}
		}
	}

	m_players.erase(temp.first); //הורדת העיגול שלי מהשחקנים האחרים

	m_me->setId(temp.first);//עדכון העיגול שלי
	m_me->setTexture(images[image]);
	m_me->setPosition(temp.second);
	m_me->setCenter(temp.second + Vector2f{ NEW_PLAYER,NEW_PLAYER });

	std::cout << std::string(name) << '\n';
}

//====================================================================================
//================================     PLAY     ======================================
//====================================================================================
unsigned Game::play(sf::RenderWindow &w, const Images &images, const Fonts &fonts)
{
	m_socket.setBlocking(false);

	sf::Packet packet;
	sf::Event event;

	while (m_me->getLive())
	{
		w.pollEvent(event);
		auto speed = TimeClass::instance().RestartClock();

		//תזוזה של השחקן
		if (m_receive) // אם הוא קלט את התזוזה הקודמת שלו
			if (event.type == sf::Event::EventType::KeyPressed)
				updateMove(speed);

		//קבלת מידע מהשרת
		receiveChanges(images, fonts);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return m_me->getScore();
		m_score.setScore(m_me->getScore());
		display(w);
	}

	return m_me->getScore();
}

//====================================================================================
//===========================      UPDATE MOVE       =================================
//====================================================================================
//מחזיר שקר אם מתתי
void Game::updateMove(float speed)
{
	sf::Packet packet;
	packet.clear();

	if (m_me->legalMove(speed))
	{
		std::vector<Uint32> deleted;
		m_me->collision(deleted, m_objectsOnBoard, m_players, m_me.get());

		if (!m_me->getLive())
			deleted.push_back(m_me->getId()); // אם מתתי

		//שליחת אובייקט זמני
		packet << m_me->getId() << m_me->getRadius() << m_me->getPosition() << deleted;

		if (m_socket.send(packet) != sf::TcpSocket::Done)
			std::cout << "no sending data\n";

		if (!m_me->getLive())
			Sleep(100);

		m_receive = false;
	}
}

//====================================================================================
//===========================      RECEIVE DATA      =================================
//====================================================================================
//מחזיר שקר אם מתתי
void Game::receiveChanges(const Images &images, const Fonts &fonts)
{
	sf::Packet packet;

	m_socket.receive(packet);
	while (!packet.endOfPacket())
	{
		std::pair<Uint32, sf::Vector2f> temp;
		if (!(packet >> temp))
			continue;
		std::vector<Uint32> del;

		if (temp.first >= FOOD_LOWER && temp.first <= BOMBS_UPPER) // אוכל או פצצות חדשות
			m_objectsOnBoard.insert(temp, images);

		else if (temp.first >= PLAYER_LOWER && temp.first <= PLAYER_UPPER)// שחקן
		{
			if (temp.first == m_me->getId())// השחקן שלי
				m_receive = true;

			else if (m_players.find(temp.first) != m_players.end())// תזוזה של שחקן (שחקן קיים..)י
			{
				m_players[temp.first]->setPosition(temp.second);
				m_players[temp.first]->setCenter();
				m_players[temp.first]->collision(del, m_objectsOnBoard, m_players, m_me.get());
			}

			else // שחקן חדש
				addPlayer(temp, packet, images, fonts);
		}
	}

	deleteDeadPlayer(m_players);
}
//------------------------------------------------------------------------------------
void Game::addPlayer(const std::pair<Uint32, sf::Vector2f> &temp, sf::Packet &packet, const Images &images, const Fonts &fonts)
{
	Uint32 image;
	sf::String name;
	packet >> image >> name;
	m_players.emplace(temp.first, std::make_unique<OtherPlayers>(temp.first, images[image], fonts[SETTINGS], NEW_PLAYER, temp.second, name));
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
//=============================================================================================
void Game::draw(sf::RenderWindow &w) const {
	for (auto &x : m_objectsOnBoard)
		w.draw(*x.second.get());

	for (auto &x : m_players)
	{
		w.draw(*x.second.get());
		w.draw(x.second->getName());
	}
	w.draw(*m_me.get());
}
//--------------------------------------------------------------------------
void Game::display(sf::RenderWindow &w)
{
	w.clear();
	//-------------------- רקע ---------------------
	auto pos = setView(w);
	m_view.setCenter(pos);
	w.setView(m_view);
	w.draw(m_background);
	draw(w);
	w.draw(m_me->getName());
	
	w.setView(m_minimap);
	w.draw(m_minimapBackground);
	draw(w);
	////------------------- ניקוד --------------------
	m_view.setCenter(float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2));
	w.setView(m_view);
	w.draw(m_score);


	w.display();

}


//*************************************************************************************
//****************************    PLAYER FUNCTION   ***********************************
//*************************************************************************************
// מחזיר שקר אם מתתי
void Player::collision(std::vector<Uint32> &deleted, Maps &objectsOnBoard, std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players, Player *me)
{
	checkPlayers(deleted, players, me);
	checkFoodAndBomb(deleted, objectsOnBoard);
}
//--------------------------------------------------------------------------
void Player::checkPlayers(std::vector<Uint32> &deleted, std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>>& players, Player *me)
{
	for (auto &player : players)
	{
		if (player.second->getId() == getId())
			continue;

		if (circlesCollide(player.second.get()))
			if (getRadius() > player.second->getRadius()) //אם היתה התנגשות בין שניים אחרים והאחר מת
			{
				newRadius(player.second.get());
				player.second->setLive(false); //מחיקה, השחקן יודע שהוא מת
				deleted.push_back(player.first);
			}
			else
				m_live = false; // אם השחקן שקרא לפונקציה מת
	}


	if (dynamic_cast<OtherPlayers*>(this)) //בדיקה של שחקן נוכחי מול השחקן שלי
	{
		if (circlesCollide(me))
			if (getRadius() > me->getRadius())
			{
				me->setLive(false);
				newRadius(me);
			}
			else
			{
				m_live = false; //השחקן שמולי מת
				me->newRadius(this);
			}
	}

	deleteDeadPlayer(players);
}
//--------------------------------------------------------------------------
void Player::checkFoodAndBomb(std::vector<Uint32> &deleted, Maps &objectsOnBoard)
{
	std::set<Uint32> check = objectsOnBoard.colliding(getCenter(), getRadius());

	for (auto it : check) //מחיקה של אוכל ופצצות והוספה לוקטור
		if (circlesCollide(objectsOnBoard[it].get()))
		{
			newRadius(objectsOnBoard[it].get());
			objectsOnBoard.eraseFromData(it);
			deleted.push_back(it);
		}

	if (getRadius() < NEW_PLAYER)
		m_live = false;
}