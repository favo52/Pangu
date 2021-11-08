#pragma once

#include <SFML/Graphics.hpp>

namespace pangu
{
	class Ball
	{
	public:
		Ball();
		~Ball();

		sf::CircleShape* getShape() { return &m_ball; }

	private:
		sf::CircleShape m_ball;
		
	private:

		void centerOrigin(sf::CircleShape& circle);
	};
}