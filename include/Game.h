#pragma once

#include "Paddle.h"
#include "Ball.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace pangu
{
	// Settings
	constexpr unsigned int W_WIDTH{ 1280 };
	constexpr unsigned int W_HEIGHT{ 720 };

	constexpr unsigned int MAX_SCORE{ 5 };
	constexpr float PADDLE_SPEED{ 6000.0f };

	enum class GameMode
	{
		STARTMEMU,
		GAMEPLAY,
		GAMEOVER,
		PAUSE
	};

	enum class MenuOption
	{
		SINGLEPLAYER,
		MULTIPLAYER,
		QUIT
	};

	MenuOption operator++(MenuOption& menuOption);
	MenuOption operator--(MenuOption& menuOption);

	enum class GameOverOption
	{
		PLAYAGAIN,
		EXIT
	};

	GameOverOption operator++(GameOverOption& option);

	class Game
	{
	public:


	public:
		Game();
		~Game();

		void run();

	private: // functions
		void processEvents();
		void update(sf::Time dt);
		void render();

		void updateStatistics(sf::Time dt);
		void handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed);

		void resetPlayers();
		void resetBall();
		void resetScores();

		void loadMenus();
		void loadStage();

	private: // variables
		static const sf::Time TimePerFrame;

		sf::RenderWindow m_Window;

		GameMode m_CurrentGamemode;
		MenuOption m_MenuOption;
		GameOverOption m_GameOverOption;

		Paddle m_Player1;
		Paddle m_Player2;
		Ball m_Ball;
		bool m_AI;

		sf::RectangleShape m_ArenaBackground;
		sf::RectangleShape m_ArenaNet;
		sf::FloatRect m_Bounds;

		sf::Font m_Font;

		// FPS counter
		sf::Text m_StatisticsText;
		sf::Time m_StatisticsUpdateTime;
		std::size_t m_StatisticsNumFrames;

		// Menu text
		sf::Text m_TitleText;
		sf::Text m_sp;
		sf::Text m_mp;
		sf::Text m_QuitText;
		sf::Text m_Playagain;
		sf::Text m_PauseText;

		// Score text
		sf::Text m_Score1Text;
		sf::Text m_Score2Text;
		sf::Text m_WinnerText;
		sf::Text m_WinMsg;

		// Win conditions
		unsigned int m_P1Score;
		unsigned int m_P2Score;
		unsigned int m_Winner;

		// Keyboard book keeping
		bool m_isKeyEnter;
		bool m_isKeyW;
		bool m_isKeyS;
		bool m_isKeyUp;
		bool m_isKeyDown;
		bool m_isKeyEsc;
	};
}
