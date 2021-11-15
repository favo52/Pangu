#include "States/TitleState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Pangu
{
	TitleState::TitleState(StateStack& stack, Context context) :
		State{ stack, context },
		m_Text{},
		m_ShowText{ true },
		m_TextEffectTime{ sf::Time::Zero }
	{
		//m_BackgroundSprite.setTexture(context.textures->Get(Textures::Arena));

		m_Text.setFont(context.fonts->Get(Fonts::Minecraft));
		m_Text.setString("Press any key to start...");
		m_Text.setCharacterSize(50u);
		centerOrigin(m_Text);
		m_Text.setPosition(context.window->getDefaultView().getSize() / 2.0f);
	}

	void TitleState::Draw()
	{
		sf::RenderWindow& window = *GetContext().window;
		//window.draw(m_BackgroundSprite);

		if (m_ShowText)
			window.draw(m_Text);
	}

	bool TitleState::Update(sf::Time dt)
	{
		m_TextEffectTime += dt;

		if (m_TextEffectTime >= sf::seconds(0.5f))
		{
			m_ShowText = !m_ShowText;
			m_TextEffectTime = sf::Time::Zero;
		}

		return true;
	}

	bool TitleState::HandleEvent(const sf::Event& event)
	{
		// If any key is pressed, tigger the next screen
		if (event.type == sf::Event::KeyPressed)
		{
			RequestStackPop();
			RequestStackPush(States::Menu);
		}

		return true;
	}
}
