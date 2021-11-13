#include "Game.h"
#include "Utility.h"

#include <SFML/Window/Event.hpp>

namespace pangu
{
    const sf::Time Game::TimePerFrame = sf::seconds(1.0f / 60.0f);

    Game::Game() :
        m_Window{ sf::VideoMode(W_WIDTH, W_HEIGHT), "Pangu!", sf::Style::Close },
        m_CurrentGamemode{ GameMode::STARTMEMU },
        m_MenuOption{ MenuOption::SINGLEPLAYER },
        m_GameOverOption{ GameOverOption::PLAYAGAIN },
        m_Player1{ sf::Vector2f(40.0f, 150.0f), sf::Color(150, 111, 51) },
        m_Player2{ sf::Vector2f(40.0f, 150.0f), sf::Color(150, 111, 51) },
        m_Ball{ 10.0f, sf::Color::Green },
        m_AI{ true },
        m_ArenaBackground{ sf::Vector2f(m_Window.getSize().x, m_Window.getSize().y) },
        m_ArenaNet{ sf::Vector2f(6.0f, m_Window.getSize().y) },
        m_Bounds{ m_ArenaBackground.getLocalBounds() },
        m_P1Score{ 0 },
        m_P2Score{ 0 },
        m_Winner{ 0 },
        m_isKeyEnter{ false },
        m_isKeyW{ false },
        m_isKeyS{ false },
        m_isKeyUp{ false },
        m_isKeyDown{ false },
        m_isKeyEsc{ false }
    {
        // load font
        if (!m_Font.loadFromFile("resources/Minecraft.ttf"))
            throw std::runtime_error("Unable to load font.");

        loadMenus();
        loadStage();
        
        m_StatisticsText.setFont(m_Font);
        m_StatisticsText.setPosition(5.0f, 5.0f);
        m_StatisticsText.setCharacterSize(10u);
    }

    Game::~Game()
    {
    }

    void Game::run()
    {
        sf::Clock clock;
        sf::Time timeSinceLastUpdate = sf::Time::Zero;
        
        while (m_Window.isOpen())
        {
            sf::Time dt = clock.restart();
            timeSinceLastUpdate += dt;
            while (timeSinceLastUpdate > TimePerFrame)
            {
                timeSinceLastUpdate -= TimePerFrame;

                processEvents();

                if (m_CurrentGamemode == GameMode::GAMEPLAY)
                    update(TimePerFrame);
            }

            updateStatistics(dt);
            render();
        }
    }

    void Game::processEvents()
    {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    m_Window.close();
                    break;
                case sf::Event::KeyPressed:
                    handlePlayerInput(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    handlePlayerInput(event.key.code, false);
                    break;
            } // end event.type
        }
    }

    void Game::render()
    {
        m_Window.clear();

        switch (m_CurrentGamemode)
        {
        case pangu::GameMode::STARTMEMU:
        {
            // draw menu text
            m_Window.draw(m_TitleText);
            m_Window.draw(m_sp);
            m_Window.draw(m_mp);
            m_Window.draw(m_QuitText);

            switch (m_MenuOption)
            {
            case MenuOption::SINGLEPLAYER:
            {
                m_AI = true;

                m_sp.setFillColor(sf::Color::Red);
                m_mp.setFillColor(sf::Color::White);
                m_QuitText.setFillColor(sf::Color::White);
                if (m_isKeyEnter)
                    m_CurrentGamemode = GameMode::GAMEPLAY;
            } break;
            case MenuOption::MULTIPLAYER:
            {
                m_AI = false;

                m_sp.setFillColor(sf::Color::White);
                m_mp.setFillColor(sf::Color::Red);
                m_QuitText.setFillColor(sf::Color::White);
                if (m_isKeyEnter)
                {
                    m_CurrentGamemode = GameMode::GAMEPLAY;
                    m_MenuOption = MenuOption::SINGLEPLAYER;
                }
            } break;
            case MenuOption::QUIT:
            {
                m_sp.setFillColor(sf::Color::White);
                m_mp.setFillColor(sf::Color::White);
                m_QuitText.setFillColor(sf::Color::Red);
                if (m_isKeyEnter)
                    m_Window.close();
            } break;
            } // end switch-case

        } break;
        case pangu::GameMode::GAMEPLAY:
        {
            m_Window.draw(m_ArenaBackground);
            m_Window.draw(m_ArenaNet);
            m_Window.draw(m_Score1Text);
            m_Window.draw(m_Score2Text);
            m_Window.draw(*m_Player1.getShape());
            m_Window.draw(*m_Player2.getShape());
            m_Window.draw(*m_Ball.getShape());
        } break;
        case pangu::GameMode::PAUSE:
        {
            // Keep background
            m_Window.draw(m_ArenaBackground);
            m_Window.draw(m_ArenaNet);
            m_Window.draw(m_Score1Text);
            m_Window.draw(m_Score2Text);
            m_Window.draw(*m_Player1.getShape());
            m_Window.draw(*m_Player2.getShape());
            m_Window.draw(*m_Ball.getShape());

            sf::RectangleShape pauseScreen(sf::Vector2f(m_Window.getSize().x, m_Window.getSize().y));
            pauseScreen.setFillColor(sf::Color(0.0f, 0.0f, 0.0f, 150u));

            // Draw pause
            m_Playagain.setString("CONTINUE");
            centerOrigin(m_Playagain);

            m_Window.draw(pauseScreen);
            m_Window.draw(m_PauseText);
            m_Window.draw(m_Playagain);
            m_Window.draw(m_QuitText);

            if (m_GameOverOption == GameOverOption::PLAYAGAIN)
            {
                m_Playagain.setFillColor(sf::Color::Red);
                m_QuitText.setFillColor(sf::Color::White);
                if (m_isKeyEnter)
                    m_CurrentGamemode = GameMode::GAMEPLAY;
            }
            else if (m_GameOverOption == GameOverOption::EXIT)
            {
                m_Playagain.setFillColor(sf::Color::White);
                m_QuitText.setFillColor(sf::Color::Red);
                if (m_isKeyEnter)
                {
                    m_CurrentGamemode = GameMode::STARTMEMU;
                    resetScores();
                    resetBall();
                    resetPlayers();
                    m_GameOverOption = GameOverOption::PLAYAGAIN;
                }
            }

            m_isKeyEnter = false;
        } break;
        case pangu::GameMode::GAMEOVER:
        {
            if (m_Winner == 1)
                m_WinnerText.setString("PLAYER ONE");
            else
                m_WinnerText.setString("PLAYER TWO");
            centerOrigin(m_WinnerText);
            m_Playagain.setString("PLAY AGAIN");
            centerOrigin(m_Playagain);

            m_Window.draw(m_WinnerText);
            m_Window.draw(m_WinMsg);
            m_Window.draw(m_Playagain);
            m_Window.draw(m_QuitText);

            if (m_GameOverOption == GameOverOption::PLAYAGAIN)
            {
                m_Playagain.setFillColor(sf::Color::Red);
                m_QuitText.setFillColor(sf::Color::White);
                if (m_isKeyEnter)
                    m_CurrentGamemode = GameMode::GAMEPLAY;
            }
            else if (m_GameOverOption == GameOverOption::EXIT)
            {
                m_Playagain.setFillColor(sf::Color::White);
                m_QuitText.setFillColor(sf::Color::Red);
                if (m_isKeyEnter)
                    m_Window.close();
            }

            resetScores();
            resetBall();
            resetPlayers();
        } break;
        default:
            break;
        } // end switch-case

        m_Window.draw(m_StatisticsText);
        m_Window.display();
    }

    void Game::update(sf::Time dt)
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
            if (m_Ball.getShape()->getPosition().x > (m_Bounds.width / 2.0f) - 150.0f)
            {
                // Follow ball
                if (m_Ball.getShape()->getPosition().y > (m_Player2.getShape()->getPosition().y) + 20.0f)
                    *m_Player2.getAccelerationPtr() -= 7000.0f;
                if (m_Ball.getShape()->getPosition().y < (m_Player2.getShape()->getPosition().y) - 20.0f)
                    *m_Player2.getAccelerationPtr() += 7000.0f;
            }

            // Move towards center when ball is on Player's side
            if (m_Player2.getShape()->getPosition().y != m_Bounds.height / 2.0f)
                (m_Player2.getShape()->getPosition().y + 10.0f && m_Player2.getShape()->getPosition().y - 10.0f < m_Bounds.height / 2.0f) ?
                *m_Player2.getAccelerationPtr() -= 1000.0f : *m_Player2.getAccelerationPtr() += 1000.0f;
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
            m_Winner = 1;
            m_CurrentGamemode = GameMode::GAMEOVER;
        }
        if (m_P2Score == MAX_SCORE)
        {
            m_Winner = 2;
            m_CurrentGamemode = GameMode::GAMEOVER;
        }
    }

    void Game::updateStatistics(sf::Time dt)
    {
        // FPS counter
        m_StatisticsUpdateTime += dt;
        m_StatisticsNumFrames += 1;

        if (m_StatisticsUpdateTime >= sf::seconds(1.0f))
        {
            m_StatisticsText.setString("FPS: " + std::to_string(m_StatisticsNumFrames));

            m_StatisticsUpdateTime -= sf::seconds(1.0f);
            m_StatisticsNumFrames = 0;
        }
    }

    void Game::handlePlayerInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        switch (key)
        {
        case sf::Keyboard::Escape:
            m_isKeyEsc = isPressed;
            if (m_CurrentGamemode == GameMode::STARTMEMU && m_isKeyEsc)
                m_Window.close();
            if (m_CurrentGamemode == GameMode::GAMEPLAY && m_isKeyEsc)
                m_CurrentGamemode = GameMode::PAUSE;
            break;
        case sf::Keyboard::Up:
            m_isKeyUp = isPressed;
            if (m_CurrentGamemode == GameMode::STARTMEMU && m_isKeyUp)
                --m_MenuOption;
            if ((m_CurrentGamemode == GameMode::GAMEOVER || m_CurrentGamemode == GameMode::PAUSE) && m_isKeyUp)
                ++m_GameOverOption;
            break;
        case sf::Keyboard::Down:
            m_isKeyDown = isPressed;
            if (m_CurrentGamemode == GameMode::STARTMEMU && m_isKeyDown)
                ++m_MenuOption;
            if ((m_CurrentGamemode == GameMode::GAMEOVER || m_CurrentGamemode == GameMode::PAUSE) && m_isKeyDown)
                ++m_GameOverOption;
            break;
        case sf::Keyboard::W:
                m_isKeyW = isPressed;
            break;
        case sf::Keyboard::S:
                m_isKeyS = isPressed;
            break;
        case sf::Keyboard::Enter:
            m_isKeyEnter = isPressed;
            break;
        } // end event.key.code
    }

    void Game::resetBall()
    {
        m_Ball.setVelocity(m_Ball.getVelocity().x * -1, 0.0f);
        m_Ball.getShape()->setPosition(sf::Vector2f(m_Window.getSize().x / 2.0f, m_Window.getSize().y / 2.0f));
    }

    void Game::resetScores()
    {
        m_P1Score = 0;
        m_P2Score = 0;
        m_Score1Text.setString(std::to_string(m_P1Score));
        m_Score2Text.setString(std::to_string(m_P2Score));
    }

    void Game::resetPlayers()
    {
        m_Player1.getShape()->setPosition(sf::Vector2f(75.0f, m_Window.getSize().y / 2.0f));
        m_Player2.getShape()->setPosition(sf::Vector2f(m_Window.getSize().x - 75.0f, m_Window.getSize().y / 2.0f));

        m_Player1.setVelocity(0.0f, 0.0f);
        m_Player2.setVelocity(0.0f, 0.0f);
    }

    void Game::loadMenus()
    {
        // PONGU
        setText(m_TitleText, m_Font, "PONGU!", 100, sf::Color::White);
        centerOrigin(m_TitleText);
        m_TitleText.setPosition(sf::Vector2f((m_Window.getSize().x / 2.0f), m_Window.getSize().y - 500.0f));

        // Singleplayer text
        setText(m_sp, m_Font, "SINGLEPLAYER", 30, sf::Color::White);
        centerOrigin(m_sp);
        m_sp.setPosition(sf::Vector2f((m_Window.getSize().x / 2.0f), m_Window.getSize().y - 350.0f));

        // Multiplayer text
        setText(m_mp, m_Font, "MULTIPLAYER", 30, sf::Color::White);
        centerOrigin(m_mp);
        m_mp.setPosition(sf::Vector2f((m_Window.getSize().x / 2.0f), m_Window.getSize().y - 300.0f));
        
        // Quit text
        setText(m_QuitText, m_Font, "QUIT", 30, sf::Color::White);
        centerOrigin(m_QuitText);
        m_QuitText.setPosition(sf::Vector2f((m_Window.getSize().x / 2.0f), m_Window.getSize().y - 200.0f));

        // Pause text
        setText(m_PauseText, m_Font, "PAUSE", 100, sf::Color::White);
        centerOrigin(m_PauseText);
        m_PauseText.setPosition(sf::Vector2f((m_Window.getSize().x / 2.0f), m_Window.getSize().y - 500.0f));
    }

    void Game::loadStage()
    {
        // Background
        m_ArenaBackground.setFillColor(sf::Color(44, 53, 57));
        m_ArenaBackground.setOutlineThickness(-50);
        m_ArenaBackground.setOutlineColor(sf::Color(12, 9, 10));

        // Center net
        m_ArenaNet.setSize(sf::Vector2f(6.0f, m_Window.getSize().y));
        centerOrigin(m_ArenaNet);
        m_ArenaNet.setPosition(sf::Vector2f(m_Window.getSize().x / 2.0f, m_Window.getSize().y / 2.0f));
        m_ArenaNet.setFillColor(sf::Color(12, 9, 10));

        // Ball
        m_Ball.getShape()->setPosition(sf::Vector2f(m_Window.getSize().x / 2.0f, m_Window.getSize().y / 2.0f));
        
        // Players
        m_Player1.getShape()->setPosition(sf::Vector2f(75.0f, m_Window.getSize().y / 2.0f));
        m_Player2.getShape()->setPosition(sf::Vector2f(m_Window.getSize().x - 75.0f, m_Window.getSize().y / 2.0f));

        // Scoreboard
        setText(m_Score1Text, m_Font, "0", 30, sf::Color::White);
        centerOrigin(m_Score1Text);
        m_Score1Text.setPosition((m_Window.getSize().x / 2.0f) - 75.0f, (m_Window.getSize().y / 2.0f) - 335.0f);

        setText(m_Score2Text, m_Font, "0", 30, sf::Color::White);
        centerOrigin(m_Score2Text);
        m_Score2Text.setPosition((m_Window.getSize().x / 2.0f) + 75.0f, (m_Window.getSize().y / 2.0f) - 335.0f);

        // End 
        setText(m_WinnerText, m_Font, "Player", 80, sf::Color::White);
        m_WinnerText.setPosition(m_Window.getSize().x / 2.0f, (m_Window.getSize().y / 2.0f) - 200.0f);

        setText(m_WinMsg, m_Font, "WON!", 80, sf::Color::White);
        centerOrigin(m_WinMsg);
        m_WinMsg.setPosition(m_Window.getSize().x / 2.0f, (m_Window.getSize().y / 2.0f) - 100.0f);

        setText(m_Playagain, m_Font, "PLAY AGAIN", 30, sf::Color::White);
        centerOrigin(m_Playagain);
        m_Playagain.setPosition(m_Window.getSize().x / 2.0f, (m_Window.getSize().y / 2.0f) + 100.0f);
    }

    MenuOption operator++(MenuOption& mOption)
    {
        return mOption = ((mOption == MenuOption::QUIT) ? // if
            MenuOption::SINGLEPLAYER :
            MenuOption(int(mOption) + 1));
    }

    MenuOption operator--(MenuOption& mOption)
    {
        return mOption = ((mOption == MenuOption::SINGLEPLAYER) ? // if
            MenuOption::QUIT :
            MenuOption(int(mOption) - 1));
    }

    GameOverOption operator++(GameOverOption& mOption)
    {
        return mOption = ((mOption == GameOverOption::PLAYAGAIN) ? // if
            GameOverOption::EXIT :
            GameOverOption::PLAYAGAIN);
    }

}