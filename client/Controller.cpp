
#ifdef _DEBUG
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#elif defined(NDEBUG)
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#else
#error "Unrecognized configuration!"
#endif

#include "Controller.h"
#include <Windows.h>//Sleep

#define GAME_OVER -1
#define START_GAME 0
#define EXIT 3
#define SETTINGS_SCREEN 1




//============================================================================
template<typename T>//this function draws the board for all vectors
void draw(sf::RenderWindow& window, const T & vec) {
	for (auto& tile : vec)
		tile->display(window);
}

//====================== main function =====================================================
int main() {

	srand(unsigned(time(NULL)));
	auto controller = std::make_unique<Controller>();
	controller->run();

	return 0;

}
/*****************************************************************************************
				basic Controller functions
**********************************************************************************************/
//===================== constructor ===================================
Controller::Controller() :m_fonts(), m_Menus(m_fonts) {
	//m_music.openFromFile("sounds/menu.ogg");
	//m_music.setLoop(true);
	//m_music.play();
	makeScreen();
}
/************************************************************************
					Controller running functions

************************************************************************/
//=======================================================================
void menuWindow(sf::RenderWindow& window) {
	window.create(sf::VideoMode{ unsigned(SCREEN_WIDTH), unsigned(SCREEN_HEIGHT) }, "Agar.io", sf::Style::None);
}
//========================= run =====================================
void Controller::run() {

	sf::RenderWindow window;
	menuWindow(window);

	//for the drawMouse
	window.setJoystickThreshold(10000);

	// Menu window
	MenuEvents(window); //draw menu window and get events

}
//======================== while window is open =================================
void Controller::MenuEvents(sf::RenderWindow& window) {

	while (window.isOpen())
	{
		events(window);  //get event from user
		if (m_Menus[START_GAME]->getPressed())
			play(window);//if game is over (no levels left)

		window.clear();
		window.draw(m_images.getImage(BACKGROUND1));//background image of openning screen
		window.draw(m_Menus.getRec());// background menu rectangle
		draw(window, m_Menus);		  // buttons of menu
		m_screeninfo[(*m_Menus.getIteratorToCurrentPressed())->getPlace()]->display(window);
		window.display();
	}
}
//==========================================================================================================
void Controller::endLevelScreen(sf::RenderWindow& w, sf::View& v, unsigned score) {
	v.setCenter(float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2);

	auto position = v.getCenter();
	position.x -= float(SCREEN_WIDTH / 2);
	position.y -= float(SCREEN_HEIGHT / 2);
	//auto position = v.getInverseTransform();

	sf::RectangleShape screen{ { float(SCREEN_WIDTH),float(SCREEN_HEIGHT) } };
	screen.setPosition(position);
	screen.setFillColor(sf::Color(0, 0, 0, 130));
	sf::Text m_l{ "GAME OVER",m_fonts[LOGO],150 };
	sf::Text m_s{ "Your score is: " + std::to_string(score),m_fonts[MENU],50 };

	auto x = (float(SCREEN_WIDTH) - m_l.getGlobalBounds().width) / 2;
	auto y = (float(SCREEN_HEIGHT) - m_l.getGlobalBounds().height) / 6;
	m_l.setPosition({ x, y });
	m_l.setFillColor(sf::Color::Yellow);
	m_l.setOutlineThickness(8);
	m_l.setOutlineColor(sf::Color(220, 220, 220, 100));

	x = (float(SCREEN_WIDTH) - m_s.getGlobalBounds().width) / 2;
	y = (float(SCREEN_HEIGHT) - m_s.getGlobalBounds().height) / 1.5;
	m_s.setPosition({ x, y });
	m_s.setFillColor(sf::Color(255, 182, 193));
	w.setView(v);

	w.draw(screen);
	w.draw(m_l);
	w.draw(m_s);
	w.display();

	Sleep(5000);
}
//========================= start playing ====================================
//if user pressed "Start"
void Controller::play(sf::RenderWindow& window) {
	sf::View view(sf::FloatRect{ 0, 0, float(SCREEN_WIDTH),float(SCREEN_HEIGHT) });
	auto it = dynamic_cast<SettingsScreen*>(m_screeninfo[SETTINGS_SCREEN].get());
	Game game{ m_images,/* m_fonts,*/it->getSelectedImage() /*,it->getName() */ ,view };
	auto score = game.play(window, m_images); //run current level
	//when the level is over resize the window to half screen

	/*display score screen*/
	endLevelScreen(window, view, score);

	//sets the start boolean to be false
	m_Menus[START_GAME]->setPressed(false);
	m_Menus.restartIt();
}
//======================= The events of menu screen ===================================
void Controller::events(sf::RenderWindow& window) {

	sf::Event event;
	window.pollEvent(event);
	static bool pressed{ false };
	bool settings = (*m_Menus.getIteratorToCurrentPressed())->getPlace() == SETTINGS_SCREEN;

	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;
	case sf::Event::MouseButtonPressed:
		//if it is in setting screen
		if (settings) {
			//if pressed on name box
			pressed = dynamic_cast<SettingsScreen*>(m_screeninfo[SETTINGS_SCREEN].get())->pressed({ float(event.mouseButton.x),float(event.mouseButton.y) });
			m_screeninfo[SETTINGS_SCREEN]->mouseEventButton({ float(event.mouseButton.x),float(event.mouseButton.y) }, true);
		}
		m_Menus.mouseEventButton(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }), true);
		break;
	case  sf::Event::TextEntered:
		if (pressed)dynamic_cast<SettingsScreen*>(m_screeninfo[SETTINGS_SCREEN].get())->enterName(event);
		break;
	case sf::Event::MouseMoved:
		m_Menus.mouseEventButton(window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }), false);
		if (settings) m_screeninfo[SETTINGS_SCREEN]->mouseEventButton({ float(event.mouseMove.x),float(event.mouseMove.y) }, false);
		break;
	}

}

//================================================================================
void Controller::makeScreen() {
	m_screeninfo.push_back(std::make_unique<Logo>(m_fonts[fonts::LOGO]));
	m_screeninfo.push_back(std::make_unique<SettingsScreen>(m_fonts[fonts::SETTINGS], m_images));
	m_screeninfo.push_back(std::make_unique<HelpScreen>(m_fonts[fonts::HELP], m_Menus.getGlobalBounds().width));

}

//===================================================================================
