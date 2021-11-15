#pragma once

#include "State.h"
#include "Ball.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Pangu
{
	class GameState : public State
	{
	public:
		GameState(StateStack& stack, Context context);

		void Draw() override;
		bool Update(sf::Time dt) override;
		bool HandleEvent(const sf::Event& event) override;

	private:
		void HandlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed);

		void resetPlayers();
		void resetBall();
		void resetScores();

	private:
		Paddle m_Player1;
		Paddle m_Player2;
		Ball m_Ball;
		bool m_AI;

		sf::Sprite m_ArenaSprite;
		sf::FloatRect m_Bounds;

		// Score text
		sf::Text m_Score1Text;
		sf::Text m_Score2Text;
		sf::Text m_WinnerText;
		sf::Text m_WinMsg;

		// Win conditions
		unsigned int m_P1Score;
		unsigned int m_P2Score;
		bool& m_Winner;

		// Keyboard book keeping
		bool m_isKeyW = false;
		bool m_isKeyS = false;
		bool m_isKeyUp = false;
		bool m_isKeyDown = false;
	};
}
