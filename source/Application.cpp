#include "Application.h"
#include "Utility.h"
#include "States/TitleState.h"
#include "States/MenuState.h"
#include "States/GameState.h"
#include "States/PauseState.h"
#include "States/GameOverState.h"

#include "SFML/Window/Event.hpp"

namespace Pangu
{
	const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.0f);
	constexpr unsigned int WINDOW_WIDTH{ 1280 };
	constexpr unsigned int WINDOW_HEIGHT{ 720 };

	Application::Application() :
		m_Window{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PANGU!", sf::Style::Close },
		m_Textures{},
		m_Fonts{},
		m_Toggle{ true },
		m_StateStack{ State::Context(m_Window, m_Textures, m_Fonts, m_Toggle) },
		m_FPSText{},
		m_FPSUpdateTime{},
		m_FPSNumFrames{ 0 }
	{
		m_Window.setKeyRepeatEnabled(false);
		m_Window.setVerticalSyncEnabled(true);

		// load font
		m_Fonts.Load(Fonts::Minecraft, "resources/Minecraft.ttf");

		// load textures
		m_Textures.Load(Textures::Arena, "resources/textures/Arena.png");
		m_Textures.Load(Textures::Paddle, "resources/textures/Paddle.png");
		m_Textures.Load(Textures::Ball, "resources/textures/Ball.png");

		m_FPSText.setFont(m_Fonts.Get(Fonts::Minecraft));
		m_FPSText.setPosition(5.0f, 5.0f);
		m_FPSText.setCharacterSize(10u);

		RegisterStates();
		m_StateStack.PushState(States::Splashscreen);
	}

	void Application::Run()
	{
		sf::Clock clock;
		sf::Time elapsedTime = sf::Time::Zero;

		while (m_Window.isOpen())
		{
			sf::Time dt = clock.restart();
			elapsedTime += dt;
			while (elapsedTime > TimePerFrame)
			{
				elapsedTime -= TimePerFrame;

				ProcessInput();
				Update(TimePerFrame);

				// Check inside this loop, because stack might be empty before update() call
				if (m_StateStack.IsEmpty())
					m_Window.close();
			}

			UpdateFPSCounter(dt);
			Render();
		}
	}

	void Application::ProcessInput()
	{
		sf::Event e;
		while (m_Window.pollEvent(e))
		{
			m_StateStack.HandleEvent(e);

			if (e.type == sf::Event::Closed)
				m_Window.close();
		}
	}

	void Application::Update(sf::Time dt)
	{
		m_StateStack.Update(dt);
	}

	void Application::Render()
	{
		m_Window.clear();

		m_StateStack.Draw();

		//m_Window.setView(m_Window.getDefaultView());
		m_Window.draw(m_FPSText);

		m_Window.display();
	}

	void Application::UpdateFPSCounter(sf::Time dt)
	{
		m_FPSUpdateTime += dt;
		m_FPSNumFrames += 1;
		if (m_FPSUpdateTime >= sf::seconds(1.0f))
		{
			m_FPSText.setString("FPS: " + std::to_string(m_FPSNumFrames));

			m_FPSUpdateTime -= sf::seconds(1.0f);
			m_FPSNumFrames = 0;
		}
	}

	void Application::RegisterStates()
	{
		m_StateStack.RegisterState<TitleState>(States::Splashscreen);
		m_StateStack.RegisterState<MenuState>(States::Menu);
		m_StateStack.RegisterState<GameState>(States::Gameplay);
		m_StateStack.RegisterState<PauseState>(States::Pause);
		m_StateStack.RegisterState<GameOverState>(States::Gameover);
	}
}
