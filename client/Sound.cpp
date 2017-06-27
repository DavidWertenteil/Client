#include "Sound.h"

Sound::Sound()
{
	m_buffer[FOOD].loadFromFile("sounds/food.wav");
	m_buffer[BOMB].loadFromFile("sounds/food.wav");

	m_sounds[FOOD].setBuffer(m_buffer[FOOD]);
	m_sounds[BOMB].setBuffer(m_buffer[BOMB]);
}
