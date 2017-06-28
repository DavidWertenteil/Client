
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
std::string m_ip;

//===================== constructor ===================================
Controller::Controller() :m_Menus() {
	std::cout << "enter server's IP: \n";
	std::cin >> m_ip;
}
/************************************************************************
					Controller running functions

************************************************************************/
//=======================================================================
void menuWindow(sf::RenderWindow& window) {
	window.create(sf::VideoMode{ unsigned(SCREEN_WIDTH), unsigned(SCREEN_HEIGHT) }, "Agar.io");// , sf::Style::None);
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

	while (true)
	{
		window.clear();
		m_Menus.restartMenu();
		window.draw(Images::instance().getImage(BACKGROUND1));//background image of openning screen
		window.draw(m_Menus.getRec());// background menu rectangle
		events(window);  //get event from user
		draw(window, m_Menus);		  // buttons of menu
		window.display();
	}
}
//======================= The events of menu screen ===================================
void Controller::events(sf::RenderWindow& window) {

	sf::Event event;
	window.pollEvent(event);

	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		m_Menus.mouseEventButton(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }), true);
		break;
	case sf::Event::MouseMoved:
		m_Menus.mouseEventButton(window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }), false);
		break;
	case sf::Event::EventType::KeyPressed:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			exit(0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			m_Menus.enter();
		break;
	}
	(*m_Menus.getIteratorToCurrentPressed())->selected(window, event);

}

//========================= start playing ====================================
//if user pressed "Start"
void Start::selected(sf::RenderWindow& window, sf::Event& ev) {
	sf::View view(sf::FloatRect{ 0, 0, float(SCREEN_WIDTH),float(SCREEN_HEIGHT) });

	load(window);//
	Game game{m_ip, m_settings.getSelectedImage()  ,view ,m_settings.getName() };
	gameOver(window, view, game.play(window));// display score screen

	 //	m_Menus.restartMenu();
}
//================================================================================
//===================================================================================
