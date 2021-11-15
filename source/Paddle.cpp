#include "Paddle.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/System/Time.hpp>

namespace Pangu
{
	Paddle::Paddle(const TextureHolder& textures) :
		m_Sprite{ textures.Get(Textures::Paddle) },
		m_AI{ true },
		m_Velocity{ sf::Vector2f(0.0f, 0.0f) },
		m_Acceleration{ 0.0f }
	{
		centerOrigin(m_Sprite);
	}

	void Paddle::update(const float& dt)
	{
		constexpr float frictionModifier{ 3.5f };
		constexpr float movementModifier{ 0.5f };

		// player friction
		float acceleration{ m_Acceleration };
		acceleration -= m_Velocity.y * frictionModifier; // decrease acceleration

		// player movement
		m_Sprite.move(sf::Vector2f(0.0f, m_Sprite.getPosition().y - (m_Sprite.getPosition().y + m_Velocity.y * dt + acceleration * dt * dt * movementModifier)));

		// speed
		m_Velocity.y = m_Velocity.y + acceleration * dt;
	}

	void Paddle::checkArenaCollision(const sf::FloatRect& bounds)
	{
		float paddleHalfSizeY{ m_Sprite.getLocalBounds().height / 2.0f };
		constexpr float arenaBorder{ 60.0f };
		constexpr float bounceModifier{ 0.25f };
		
		// player collision top and bottom
		if (m_Sprite.getPosition().y < bounds.top + (arenaBorder + paddleHalfSizeY))
		{
			m_Sprite.setPosition(sf::Vector2f(m_Sprite.getPosition().x, (bounds.top + arenaBorder) + paddleHalfSizeY));
			m_Velocity.y *= -bounceModifier; // bounce off wall
		}
		else if (m_Sprite.getPosition().y > bounds.height - (arenaBorder + paddleHalfSizeY))
		{
			m_Sprite.setPosition(sf::Vector2f(m_Sprite.getPosition().x, (bounds.height - arenaBorder) - paddleHalfSizeY));
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

	sf::Sprite* Paddle::getSprite()
	{
		return &m_Sprite;
	}

	sf::FloatRect Paddle::getPosition()
	{
		return m_Sprite.getGlobalBounds();;
	}
}
