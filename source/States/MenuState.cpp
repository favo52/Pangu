#include "States/MenuState.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include "Paddle.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace Pangu
{
	MenuState::MenuState(StateStack& stack, Context context) :
		State{ stack, context },
		m_isAI{ *context.toggle },
		m_TitleText{},
		m_MenuOptions{},
		m_CurrentOption{ MenuOptions::Singleplayer }
	{
		//sf::Texture& texture = context.textures->Get(Textures::Arena);
		sf::Font& font = context.fonts->Get(Fonts::Minecraft);

		//m_BackgroundSprite.setTexture(texture);

		setText(m_TitleText, font, "PONGU!", 100u, sf::Color::White);
		centerOrigin(m_TitleText);
		m_TitleText.setPosition((context.window->getView().getSize() / 2.0f) - sf::Vector2f(0.0f, 200));

		sf::Text singleplayerOption;
		setText(singleplayerOption, font, "SINGLEPLAYER", 30u, sf::Color::White);
		centerOrigin(singleplayerOption);
		singleplayerOption.setPosition(context.window->getView().getSize() / 2.0f);
		m_MenuOptions.push_back(singleplayerOption);

		sf::Text multiplayerOption;
		setText(multiplayerOption, font, "MULTIPLAYER", 30u, sf::Color::White);
		centerOrigin(multiplayerOption);
		multiplayerOption.setPosition(singleplayerOption.getPosition() + sf::Vector2f(0.0f, 50.0f));
		m_MenuOptions.push_back(multiplayerOption);

		sf::Text exitOption;
		setText(exitOption, font, "EXIT", 30u, sf::Color::White);
		centerOrigin(exitOption);
		exitOption.setPosition(multiplayerOption.getPosition() + sf::Vector2f(0.0f, 80.0f));
		m_MenuOptions.push_back(exitOption);

		UpdateOptionText();
	}

	void MenuState::Draw()
	{
		sf::RenderWindow& window = *GetContext().window;

		window.setView(window.getDefaultView());

		window.draw(m_TitleText);

		for (const sf::Text& text : m_MenuOptions)
			window.draw(text);
	}

	bool MenuState::Update(sf::Time dt)
	{
		return true;
	}

	bool MenuState::HandleEvent(const sf::Event& event)
	{
		if (event.type != sf::Event::KeyPressed)
			return false;

		switch (event.key.code)
		{
			case sf::Keyboard::Return:
			{
				if (m_CurrentOption == MenuOptions::Singleplayer)
				{
					m_isAI = true;
					RequestStackPop();
					RequestStackPush(States::Gameplay);
				}
				else if (m_CurrentOption == MenuOptions::Multiplayer)
				{
					m_isAI = false;
					RequestStackPop();
					RequestStackPush(States::Gameplay);
				}
				else if (m_CurrentOption == MenuOptions::Exit)
				{
					// The exit option was chosen, by removing itself,
					// the stack will be empty, and the game will know it is time to close.
					RequestStackPop();
				}
			} break;
			case sf::Keyboard::Up:
			{
				--m_CurrentOption;
				UpdateOptionText();
			} break;
			case sf::Keyboard::Down:
			{
				++m_CurrentOption;
				UpdateOptionText();
			} break;
		}

		return true;
	}

	void MenuState::UpdateOptionText()
	{
		if (m_MenuOptions.empty())
			return;

		// White all texts
		for (sf::Text& text : m_MenuOptions)
			text.setFillColor(sf::Color::White);

		// Red the selected text
		m_MenuOptions[(int)m_CurrentOption].setFillColor(sf::Color::Red);
	}
	
	MenuState::MenuOptions operator++(MenuState::MenuOptions& menuOption)
	{
		return menuOption = ((menuOption == MenuState::MenuOptions::Exit) ?
			MenuState::MenuOptions::Singleplayer :
			MenuState::MenuOptions((int)menuOption + 1));
	}

	MenuState::MenuOptions operator--(MenuState::MenuOptions& menuOption)
	{
		return menuOption = ((menuOption == MenuState::MenuOptions::Singleplayer) ?
			MenuState::MenuOptions::Exit :
			MenuState::MenuOptions((int)menuOption - 1));
	}
}
