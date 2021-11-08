#pragma once

#include <SFML/Graphics.hpp>

#include <Ball.h>
#include <Paddle.h>

#include <memory>

namespace pangu
{
	// Window default size
	constexpr unsigned int W_WIDTH{ 1280 };
	constexpr unsigned int W_HEIGHT{ 720 };

	enum class GameMode
	{
		STARTMEMU, GAMEPLAY, PAUSEMENU, GAMEFINISH
	};

	enum class MenuOption
	{
		SINGLEPLAYER, MULTIPLAYER, QUIT
	};
	MenuOption operator++(MenuOption& menuOption);
	MenuOption operator--(MenuOption& menuOption);

	class Game
	{
	public:
		Game();
		~Game();

		void run();
		void quit();

	private: // variables
		std::unique_ptr<sf::RenderWindow> m_window;

		GameMode m_currentGamemode;
		MenuOption m_menuOption;

		// stage
		sf::RectangleShape m_arenaBackground;
		sf::RectangleShape m_arenaNet;

		// ball
		Ball m_ball;

		// players
		Paddle m_player1;
		Paddle m_player2;

		sf::Font m_font;

		// menu text
		sf::Text m_title;
		sf::Text m_sp;
		sf::Text m_mp;
		sf::Text m_quit;

		bool m_isRunning;
		bool m_isKeyEnter;

	private: // functions
		void processEvents();
		void simulate();
		void update();

		// prepare text
		void loadStartMenu();

		void setText(sf::Text& text, const sf::Font& font, const std::string& str, unsigned int size, sf::Color color);
		void centerOrigin(sf::Text& text);
		void centerOrigin(sf::RectangleShape& text);
	};
}
