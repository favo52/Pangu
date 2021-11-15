#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "States/StateStack.h"

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

namespace Pangu
{
	class Application
	{
	public:
		Application();

		void Run();

	private:
		void ProcessInput();
		void Update(sf::Time dt);
		void Render();

		void UpdateFPSCounter(sf::Time dt);
		void RegisterStates();

	private:
		static const sf::Time TimePerFrame;

		sf::RenderWindow m_Window;
		TextureHolder m_Textures;
		FontHolder m_Fonts;
		bool m_Toggle;

		StateStack m_StateStack;

		sf::Text m_FPSText;
		sf::Time m_FPSUpdateTime;
		std::size_t m_FPSNumFrames;
	};
}
