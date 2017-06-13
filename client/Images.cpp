#include "Images.h"

Images::Images() {

	startImages();

	for (int i = 0; i < Images_t::SPRITE_NUMBER; i++)
		m_image.push_back(sf::Sprite{ m_picTexture[i] });

	m_image[BACKGROUND].setTextureRect(sf::IntRect{ 0,0,3000,3000 });
	m_picTexture[BACKGROUND].setRepeated(true);

	//set menu background repeat
	m_image[BACKGROUND1].setTextureRect(sf::IntRect{ 0,0,int(SCREEN_WIDTH),int(SCREEN_HEIGHT) });
	m_picTexture[BACKGROUND1].setRepeated(true);//
}
//================================================================
void Images::startImages() {
	m_picTexture[BACKGROUND].loadFromFile("Images/background.jpg");
	m_picTexture[BACKGROUND1].loadFromFile("Images/Background1.jpg");

	m_cirTexture[color1].loadFromFile("Images/color1.png");
	m_cirTexture[color2].loadFromFile("Images/color2.png");
	m_cirTexture[color3].loadFromFile("Images/color3.png");
	m_cirTexture[color4].loadFromFile("Images/color4.png");
	m_cirTexture[color5].loadFromFile("Images/color5.png");
	m_cirTexture[color6].loadFromFile("Images/color6.png");
	m_cirTexture[color7].loadFromFile("Images/color7.png");
	m_cirTexture[color8].loadFromFile("Images/color8.png");
	m_cirTexture[color9].loadFromFile("Images/color9.png");
	m_cirTexture[color10].loadFromFile("Images/color10.png");
	m_cirTexture[color11].loadFromFile("Images/color11.png");


}