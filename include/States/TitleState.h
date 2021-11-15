#pragma once

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Pangu
{
	class TitleState : public State
	{
	public:
		TitleState(StateStack& stack, Context context);

		void Draw() override;
		bool Update(sf::Time dt) override;
		bool HandleEvent(const sf::Event& event) override;

	private:
		//sf::Sprite  m_BackgroundSprite;
		sf::Text	m_Text;

		bool		m_ShowText;
		sf::Time	m_TextEffectTime;
	};
}
