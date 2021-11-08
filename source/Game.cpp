#include <Game.h>

#include <iostream>

namespace pangu
{
    Game::Game() :
        m_window{ nullptr },
        m_currentGamemode{ GameMode::STARTMEMU },
        m_menuOption{ MenuOption::SINGLEPLAYER },
        m_ball{},
        m_player1{},
        m_player2{},
        m_isRunning{ true },
        m_isKeyEnter{ false }
    {
        // create Window
        m_window = std::unique_ptr<sf::RenderWindow>
            (new sf::RenderWindow(sf::VideoMode(W_WIDTH, W_HEIGHT), "Pangu!"));
        m_window->setVerticalSyncEnabled(true);

        // load font
        if (!m_font.loadFromFile("resources/Minecraft.ttf"))
            throw std::runtime_error("Unable to load font.");

        loadStartMenu();

        // load stage
        m_arenaBackground.setSize(sf::Vector2f(m_window->getSize().x, m_window->getSize().y));
        m_arenaBackground.setFillColor(sf::Color(44, 53, 57));
        m_arenaBackground.setOutlineThickness(-50);
        m_arenaBackground.setOutlineColor(sf::Color(12, 9, 10));
        centerOrigin(m_arenaNet);
        m_arenaNet.setPosition(sf::Vector2f(m_window->getSize().x / 2.0f, 0.0f));
        m_arenaNet.setSize(sf::Vector2f(5.0f, m_window->getSize().y));
        m_arenaNet.setFillColor(sf::Color(12, 9, 10));

        // ball
        m_ball.getShape()->setPosition(sf::Vector2f(m_window->getSize().x / 2.0f, m_window->getSize().y / 2.0f));
    }

    Game::~Game()
    {
    }

    void Game::run()
    {
        while (m_isRunning && m_window->isOpen())
        {
            processEvents();

            m_window->clear();
            simulate();
            m_window->display();
        }
    }

    void Game::quit()
    {
        m_isRunning = false;
    }

    void Game::processEvents()
    {
        sf::Event event;
        while (m_window->pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    m_window->close();
                    break;
                case sf::Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                            if (m_currentGamemode == GameMode::STARTMEMU)
                                m_window->close();
                            if (m_currentGamemode == GameMode::GAMEPLAY)
                                m_currentGamemode = GameMode::PAUSEMENU;
                            break;
                        case sf::Keyboard::Up:
                            if(m_currentGamemode == GameMode::STARTMEMU)
                                --m_menuOption;
                            break;
                        case sf::Keyboard::Down:
                            if (m_currentGamemode == GameMode::STARTMEMU)
                                ++m_menuOption;
                            break;
                        case sf::Keyboard::Enter:
                            m_isKeyEnter = true;
                            break;
                    } // end event.key.code
                } break;
                case sf::Event::KeyReleased:
                {
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Enter:
                            m_isKeyEnter = false;
                            break;
                    } // end event.key.code
                } break;
            } // end event.type
        }
    }

    void Game::simulate()
    {
        switch (m_currentGamemode)
        {
        case pangu::GameMode::STARTMEMU:
        {
            // draw text
            m_window->draw(m_title);
            m_window->draw(m_sp);
            m_window->draw(m_mp);
            m_window->draw(m_quit);

            // switch
            switch (m_menuOption)
            {
            case MenuOption::SINGLEPLAYER:
            {
                // TODO: singleplayer support
                m_sp.setFillColor(sf::Color::Red);
                m_mp.setFillColor(sf::Color::White);
                m_quit.setFillColor(sf::Color::White);
                if (m_isKeyEnter)
                    m_currentGamemode = GameMode::GAMEPLAY;
            } break;
            case MenuOption::MULTIPLAYER:
            {
                // TODO: multiplayer support
                m_sp.setFillColor(sf::Color::White);
                m_mp.setFillColor(sf::Color::Red);
                m_quit.setFillColor(sf::Color::White);
                if (m_isKeyEnter)
                    m_currentGamemode = GameMode::GAMEPLAY;
            } break;
            case MenuOption::QUIT:
            {
                m_sp.setFillColor(sf::Color::White);
                m_mp.setFillColor(sf::Color::White);
                m_quit.setFillColor(sf::Color::Red);
                if (m_isKeyEnter)
                    quit();
            } break;
            } // end switch-case
                
        } break;
        case pangu::GameMode::GAMEPLAY:
        {
            m_window->draw(m_arenaBackground);
            m_window->draw(m_arenaNet);
            m_window->draw(*m_ball.getShape());
        } break;
        case pangu::GameMode::PAUSEMENU:
        {
            m_currentGamemode = GameMode::STARTMEMU;
        } break;
        case pangu::GameMode::GAMEFINISH:
        {

        } break;
        default:
            break;
        } // end switch-case
    }

    void Game::loadStartMenu()
    {
        // PONGU
        setText(m_title, m_font, "PONGU!", 100, sf::Color::White);
        centerOrigin(m_title);
        m_title.setPosition(sf::Vector2f(((float)m_window->getSize().x / 2.0f),
                                          (float)m_window->getSize().y - 500.0f));

        // Setup Singleplayer
        setText(m_sp, m_font, "SINGLEPLAYER", 30, sf::Color::White);
        centerOrigin(m_sp);
        m_sp.setPosition(sf::Vector2f(((float)m_window->getSize().x / 2.0f),
                                       (float)m_window->getSize().y - 350.0f));

        // Setup Multiplayer
        setText(m_mp, m_font, "MULTIPLAYER", 30, sf::Color::White);
        centerOrigin(m_mp);
        m_mp.setPosition(sf::Vector2f(((float)m_window->getSize().x / 2.0f),
                                       (float)m_window->getSize().y - 300.0f));
        
        // Setup Quit
        setText(m_quit, m_font, "QUIT", 30, sf::Color::White);
        centerOrigin(m_quit);
        m_quit.setPosition(sf::Vector2f(((float)m_window->getSize().x / 2.0f),
                                         (float)m_window->getSize().y - 200.0f));
    }

    void Game::setText(sf::Text& text, const sf::Font& font, const std::string& str, unsigned int size, sf::Color color)
    {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
    }

    void Game::centerOrigin(sf::Text& text)
    {
        sf::FloatRect titleRect = text.getLocalBounds();
        text.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    }

    void Game::centerOrigin(sf::RectangleShape& rect)
    {
        sf::FloatRect titleRect = rect.getLocalBounds();
        rect.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    }

    MenuOption operator++(MenuOption& mOption)
    {
        return mOption = ((mOption == MenuOption::QUIT) ? // if
            MenuOption::SINGLEPLAYER :
            MenuOption(int(mOption) + 1)); // else
    }

    MenuOption operator--(MenuOption& mOption)
    {
        return mOption = ((mOption == MenuOption::SINGLEPLAYER) ? // if
            MenuOption::QUIT :
            MenuOption(int(mOption) - 1)); // else
    }
}