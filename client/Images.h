#pragma once

#include <SFML/Graphics.hpp>

#include <string>

enum Images_t
{
	BACKGROUND1,
	SPRITE_NUMBER, //number of images, add an image befor this line 
	color1 = 1,
	color2,
	color3,
	color4,
	color5,
	color6,
	color7,
	color8,
	color9,
	color10,
	color11,
	CIRCLE_NUMBER
};


const unsigned SCREEN_WIDTH = sf::VideoMode::getDesktopMode().width;
const unsigned SCREEN_HEIGHT = sf::VideoMode::getDesktopMode().height;

class Images
{
public:
	Images();
	const sf::Sprite& getImage(int x)const { return m_image[x]; }
	const std::vector<sf::Sprite>& getImages()const { return m_image; }

	/* set */
	void setPosition(const int i, const sf::Vector2f& position) { m_image[i].setPosition(position); }
	void setScale(const int i, const sf::Vector2f& scale) { m_image[i].setScale(scale); }

	//operator
	const sf::Sprite& operator[](int i)const { return m_image[i]; }
	sf::Sprite& operator[](int i) { return m_image[i]; }
	const sf::Texture& operator[](sf::Uint32 i) const { return m_picTexture[i]; } //????????????
	//const sf::Texture& operator[](sf::Uint32 i) const { return m_cirTexture[i]; }


private:
	void startImages();

	sf::Texture m_picTexture[Images_t::SPRITE_NUMBER];
	sf::Texture m_cirTexture[Images_t::CIRCLE_NUMBER];
	std::vector<sf::Sprite> m_image;
};