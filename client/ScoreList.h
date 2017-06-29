#pragma once
#include "Circle.h"

bool cmp(unsigned n1, unsigned n2);//for the multymap

class ScoreList
{
public:
	ScoreList();
	void display(sf::RenderWindow &w, const std::unordered_map<Uint32, std::unique_ptr<OtherPlayers>> &players, const std::unique_ptr<MyPlayer> &me);

private:
	std::multimap <unsigned, sf::String, bool(*)(unsigned, unsigned)> m_scoreList{ cmp };
	sf::Text m_text;
};