#include "States/GameState.h"
#include "Utility.h"

#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Pangu
{
    constexpr unsigned int MAX_SCORE{ 5 };
    constexpr float PADDLE_SPEED{ 2500.0f };
    constexpr float AI_CHEAT_BOOST{ 500.0f };

	GameState::GameState(StateStack& stack, Context context) :
		State{ stack, context },
        m_AI{ *context.toggle },
		m_Player1{ *context.textures },
		m_Player2{ *context.textures },
		m_Ball{ *context.textures },
        m_P1Score{ 0 },
        m_P2Score{ 0 },
        m_Winner{ *context.toggle }
	{
		m_Player2.setAI(context.toggle);

        sf::Font& font = context.fonts->Get(Fonts::Minecraft);
        
		sf::Texture& arenaTexture = context.textures->Get(Textures::Arena);
		m_ArenaSprite.setTexture(arenaTexture);
        m_Bounds = m_ArenaSprite.getLocalBounds();

        // Scoreboard
        setText(m_Score1Text, font, "0", 30u, sf::Color::White);
        centerOrigin(m_Score1Text);
        m_Score1Text.setPosition((m_Bounds.width / 2.0f) - 75.0f, (m_Bounds.height / 2.0f) - 335.0f);

        setText(m_Score2Text, font, "0", 30u, sf::Color::White);
        centerOrigin(m_Score2Text);
        m_Score2Text.setPosition((m_Bounds.width / 2.0f) + 75.0f, (m_Bounds.height / 2.0f) - 335.0f);

        // Ball
        m_Ball.getSprite()->setPosition(sf::Vector2f(m_Bounds.width / 2.0f, m_Bounds.height / 2.0f));

        // Players
        m_Player1.getSprite()->setPosition(sf::Vector2f(65.0f, m_Bounds.height / 2.0f));
        m_Player2.getSprite()->setPosition(sf::Vector2f(m_Bounds.width - 65.0f, m_Bounds.height / 2.0f));
	}

	void GameState::Draw()
	{
		sf::RenderWindow& window = *GetContext().window;

		window.draw(m_ArenaSprite);
        window.draw(m_Score1Text);
        window.draw(m_Score2Text);
		window.draw(*m_Player1.getSprite());
		window.draw(*m_Player2.getSprite());
		window.draw(*m_Ball.getSprite());
	}

	bool GameState::Update(sf::Time dt)
	{
        // Player 1 input control
        m_Player1.setAcceleration(0.0f);
        if (m_isKeyW) *m_Player1.getAccelerationPtr() += PADDLE_SPEED;
        if (m_isKeyS) *m_Player1.getAccelerationPtr() -= PADDLE_SPEED;

        // Player 2 input control
        m_Player2.setAcceleration(0.0f);
        if (!m_AI)
        {
            if (m_isKeyUp) *m_Player2.getAccelerationPtr() += PADDLE_SPEED;
            if (m_isKeyDown) *m_Player2.getAccelerationPtr() -= PADDLE_SPEED;
        }
        else // A.I.
        {
            // Move if ball is on AI's side
            if (m_Ball.getSprite()->getPosition().x > (m_Bounds.width / 2.0f) - 200.0f)
            {
                // Follow ball
                if (m_Ball.getSprite()->getPosition().y > (m_Player2.getSprite()->getPosition().y) + 20.0f)
                    *m_Player2.getAccelerationPtr() -= PADDLE_SPEED + AI_CHEAT_BOOST;
                if (m_Ball.getSprite()->getPosition().y < (m_Player2.getSprite()->getPosition().y) - 20.0f)
                    *m_Player2.getAccelerationPtr() += PADDLE_SPEED + AI_CHEAT_BOOST;
            }

            // Move towards center when ball is on Player's side
            if (m_Player2.getSprite()->getPosition().y != m_Bounds.height / 2.0f)
                (m_Player2.getSprite()->getPosition().y + 10.0f && m_Player2.getSprite()->getPosition().y - 10.0f < m_Bounds.height / 2.0f) ?
                *m_Player2.getAccelerationPtr() -= 300.0f : *m_Player2.getAccelerationPtr() += 300.0f;
        }

        // Player movement and collisions
        m_Player1.update(dt.asSeconds());
        m_Player1.checkArenaCollision(m_Bounds);

        m_Player2.update(dt.asSeconds());
        m_Player2.checkArenaCollision(m_Bounds);

        // Ball movement and collisions
        m_Ball.update(dt.asSeconds());
        m_Ball.checkArenaCollision(m_Bounds);
        m_Ball.checkPaddleCollision(m_Player1, m_Player2);

        // Score and reset ball
        if (m_Ball.isScore(m_Bounds) == 1)
        {
            resetBall();
            ++m_P1Score;
            m_Score1Text.setString(std::to_string(m_P1Score));
        }
        else if (m_Ball.isScore(m_Bounds) == 2)
        {
            resetBall();
            ++m_P2Score;
            m_Score2Text.setString(std::to_string(m_P2Score));
        }

        // Win condition
        if (m_P1Score == MAX_SCORE)
        {
            m_Winner = false;
            RequestStackPop();
            RequestStackPush(States::Gameover);
        }
        if (m_P2Score == MAX_SCORE)
        {
            m_Winner = true;
            RequestStackPop();
            RequestStackPush(States::Gameover);
        }

		return true;
	}

	bool GameState::HandleEvent(const sf::Event& event)
	{
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Escape)
                    RequestStackPush(States::Pause);

                HandlePlayerInput(event.key.code, true);
            } break;

            case sf::Event::KeyReleased:
            {
                HandlePlayerInput(event.key.code, false);
            } break;
        } // end event.type

		return true;
	}

    void GameState::HandlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        switch (key)
        {
            case sf::Keyboard::Up:
                m_isKeyUp = isPressed;
                break;
            case sf::Keyboard::Down:
                m_isKeyDown = isPressed;
                break;
            case sf::Keyboard::W:
                m_isKeyW = isPressed;
                break;
            case sf::Keyboard::S:
                m_isKeyS = isPressed;
                break;
        } // end event.key.code
    }

    void GameState::resetPlayers()
    {
        m_Player1.getSprite()->setPosition(sf::Vector2f(75.0f, m_Bounds.height / 2.0f));
        m_Player2.getSprite()->setPosition(sf::Vector2f(m_Bounds.width - 75.0f, m_Bounds.height / 2.0f));

        m_Player1.setVelocity(0.0f, 0.0f);
        m_Player2.setVelocity(0.0f, 0.0f);
    }

    void GameState::resetBall()
    {
        m_Ball.setVelocity(m_Ball.getVelocity().x * -1, 0.0f);
        m_Ball.getSprite()->setPosition(sf::Vector2f(m_Bounds.width / 2.0f, m_Bounds.height / 2.0f));
    }

    void GameState::resetScores()
    {
        m_P1Score = 0;
        m_P2Score = 0;
        m_Score1Text.setString(std::to_string(m_P1Score));
        m_Score2Text.setString(std::to_string(m_P2Score));
    }
}
