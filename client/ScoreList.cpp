#include "ScoreList.h"



bool cmp(unsigned n1, unsigned n2)
{
	return n1 > n2;
}

//===========================================================================
ScoreList::ScoreList()
{
	m_text.setFillColor(sf::Color(105, 105, 105));
	m_text.setFont(Fonts::instance()[SETTINGS]);
	m_text.setCharacterSize(27);
}
//===========================================================================
void ScoreList::display(sf::RenderWindow &w, const std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> &players, const std::unique_ptr<MyPlayer> &me)
{
	int i = 1;
	sf::String temp = "Best players:\n";
	m_scoreList.clear();

	for (auto it = players.begin(); it != players.end(); it++)
		m_scoreList.insert({ it->second->getScore(), it->second->getString() });
	m_scoreList.insert({ me->getScore(), me->getString() });

	for (auto it = m_scoreList.begin(); i <= 3 && it != m_scoreList.end(); it++, i++)
		temp += (std::to_string(i) + ". " + it->second + " " + std::to_string(it->first) + '\n');

	m_text.setPosition({ float(sf::VideoMode::getDesktopMode().width) - m_text.getGlobalBounds().width - 10, float(sf::VideoMode::getDesktopMode().height) - m_text.getGlobalBounds().height - 10 });
	m_text.setString(temp);
	w.draw(m_text);
}