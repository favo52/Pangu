#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

// Forward declaration
namespace sf
{
	class Time;
}

namespace pangu
{
	class Paddle
	{
	public:
		Paddle(const sf::Vector2f& size, const sf::Color& color);

		void update(const float& dt);
		void checkArenaCollision(const sf::FloatRect& bounds);

		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		void setAcceleration(const float& acceleration);

		const sf::Vector2f getVelocity() const;

		const float getAcceleration() const;
		float* getAccelerationPtr();

		sf::RectangleShape* getShape();

		sf::FloatRect getPosition();

	private:
		sf::RectangleShape	m_Shape;

		sf::Vector2f m_Velocity;
		float m_Acceleration;
	};
}
