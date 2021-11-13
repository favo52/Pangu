#include "Paddle.h"
#include "Utility.h"

#include <SFML/System/Time.hpp>

namespace pangu
{
	Paddle::Paddle(const sf::Vector2f& size, const sf::Color& color) :
		m_Shape{ size },
		m_Velocity{ sf::Vector2f(0.0f, 0.0f) },
		m_Acceleration{ 0.0f }
	{
		m_Shape.setFillColor(color);
		m_Shape.setOutlineThickness(-5.0f);
		m_Shape.setOutlineColor(sf::Color::Black);
		centerOrigin(m_Shape);
	}

	void Paddle::update(const float& dt)
	{
		constexpr float frictionModifier{ 10.0f };
		constexpr float movementModifier{ 0.5f };

		// player friction
		float acceleration{ m_Acceleration };
		acceleration -= m_Velocity.y * frictionModifier; // decrease acceleration

		// player movement
		m_Shape.move(sf::Vector2f(0.0f, m_Shape.getPosition().y - (m_Shape.getPosition().y + m_Velocity.y * dt + acceleration * dt * dt * movementModifier)));

		// speed
		m_Velocity.y = m_Velocity.y + acceleration * dt;
	}

	void Paddle::checkArenaCollision(const sf::FloatRect& bounds)
	{
		constexpr float arenaBorder{ 50.0f };
		constexpr float paddleHalfSizeY{ 75.0f };
		constexpr float bounceModifier{ 0.25f };

		// player collision top and bottom
		if (m_Shape.getPosition().y < bounds.top + (arenaBorder + paddleHalfSizeY))
		{
			m_Shape.setPosition(sf::Vector2f(m_Shape.getPosition().x, (bounds.top + arenaBorder) + paddleHalfSizeY));
			m_Velocity.y *= -bounceModifier; // bounce off wall
		}
		else if (m_Shape.getPosition().y > bounds.height - (arenaBorder + paddleHalfSizeY))
		{
			m_Shape.setPosition(sf::Vector2f(m_Shape.getPosition().x, (bounds.height - arenaBorder) - paddleHalfSizeY));
			m_Velocity.y *= -bounceModifier; // bounce off wall
		}
	}

	void Paddle::setVelocity(sf::Vector2f velocity)
	{
		m_Velocity = velocity;
	}

	void Paddle::setVelocity(float vx, float vy)
	{
		m_Velocity.x = vx;
		m_Velocity.y = vy;
	}

	void Paddle::setAcceleration(const float& acceleration)
	{
		m_Acceleration = acceleration;
	}

	const sf::Vector2f Paddle::getVelocity() const
	{
		return m_Velocity;
	}

	const float Paddle::getAcceleration() const
	{
		return m_Acceleration;
	}

	float* Paddle::getAccelerationPtr()
	{
		return &m_Acceleration;
	}

	sf::RectangleShape* Paddle::getShape()
	{
		return &m_Shape;
	}

	sf::FloatRect Paddle::getPosition()
	{
		return m_Shape.getGlobalBounds();;
	}
}
