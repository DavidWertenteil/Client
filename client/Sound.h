//#pragma once
//#include <SFML\Audio.hpp>
//
//enum sounds
//{
//	FOOD,
//	BOMB,
//	SOUND_NUMBER
//};
//
//class Sound
//{
//public:
//	inline static Sound& instance() { static Sound tone; return tone; }
//	void play(const sounds& s) { m_sounds[s].play(); }
//	~Sound() {}
//private:
//	Sound();
//	Sound(const Sound &s) = delete;
//	Sound& operator= (const Sound &s) = delete;
//
//	sf::Sound m_sounds[SOUND_NUMBER];
//	sf::SoundBuffer m_buffer[SOUND_NUMBER];
//};