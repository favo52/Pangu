#include "Ball.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/System/Time.hpp>

namespace Pangu
{
	Ball::Ball(const TextureHolder& textures) :
		m_Sprite{ textures.Get(Textures::Ball) },
		m_Radius{ m_Sprite.getTextureRect().height / 2.0f },
		m_Velocity{ -750.0f, 0.0f }
	{
		centerOrigin(m_Sprite);
	}

	Ball::~Ball()
	{
	}

	void Ball::update(float dt)
	{
		// Ball movement
		m_Sprite.move(m_Velocity.x * dt, m_Velocity.y * dt);
	}

	void Ball::checkArenaCollision(const sf::FloatRect& bounds)
	{
		constexpr float border{ 82.5f };

		// Ball top and bottom collision
		if (m_Sprite.getPosition().y + m_Radius < bounds.top + border)
		{
			m_Sprite.setPosition(m_Sprite.getPosition().x, (bounds.top + border) - m_Radius);
			m_Velocity.y *= -1.0f; // reverse direction
		}
		else if (m_Sprite.getPosition().y - m_Radius > bounds.height - border)
		{
			m_Sprite.setPosition(m_Sprite.getPosition().x, (bounds.height - border) + m_Radius);
			m_Velocity.y *= -1.0f; // reverse direction
		}
	}

	void Ball::checkPaddleCollision(Paddle player1, Paddle player2)
	{
		float ballPosX = m_Sprite.getPosition().x;
		float ballPosY = m_Sprite.getPosition().y;
		float paddle1PositionX{ player1.getSprite()->getPosition().x };
		float paddle1PositionY{ player1.getSprite()->getPosition().y };
		float paddle2PositionX{ player2.getSprite()->getPosition().x };
		float paddle2PositionY{ player2.getSprite()->getPosition().y };
		float paddleHalfSizeX{ player1.getSprite()->getLocalBounds().width / 2.0f };
		float paddleHalfSizeY{ player1.getSprite()->getLocalBounds().height / 2.0f };

		// Ball paddle collision
		if (aabb_vs_aabb(ballPosX, ballPosY, m_Radius, m_Radius, paddle1PositionX, paddle1PositionY, paddleHalfSizeX, paddleHalfSizeY))
		{
			m_Sprite.setPosition(paddle1PositionX + paddleHalfSizeX + m_Radius, ballPosY);
			m_Velocity.x *= -1.0f;
			m_Velocity.y = ((ballPosY - paddle1PositionY) * 2.0f - player1.getVelocity().y);
		}
		else if (aabb_vs_aabb(ballPosX, ballPosY, m_Radius, m_Radius, paddle2PositionX, paddle2PositionY, paddleHalfSizeX, paddleHalfSizeY))
		{
			m_Sprite.setPosition(paddle2PositionX - paddleHalfSizeX - m_Radius, ballPosY);
			m_Velocity.x *= -1.0f;
			m_Velocity.y = ((ballPosY - paddle2PositionY) * 2.0f - player2.getVelocity().y);
		}
	}

	int Ball::isScore(const sf::FloatRect& bounds)
	{
		float posX = m_Sprite.getPosition().x;
		constexpr float border{ 45.0f };

		// If ball touches side borders
		if (posX + m_Radius > bounds.width - border)
			return 1; // score player 1
		else if (posX - m_Radius < bounds.left + border)
			return 2; // score player 2
		
		return 0; // no score
	}

	void Ball::setVelocity(sf::Vector2f velocity)
	{
		m_Velocity = velocity;
	}

	void Ball::setVelocity(float vx, float vy)
	{
		m_Velocity.x = vx;
		m_Velocity.y = vy;
	}

	const sf::Vector2f Ball::getVelocity() const
	{
		return m_Velocity;
	}

	sf::Sprite* Ball::getSprite()
	{
		return &m_Sprite;
	}

	sf::FloatRect Ball::getPosition()
	{
		return m_Sprite.getGlobalBounds();
	}

	bool Ball::aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y)
	{
		return (p1x + hs1x > p2x - hs2x
			&& p1x - hs1x < p2x + hs2x
			&& p1y + hs1y > p2y - hs2y
			&& p1y + hs1y < p2y + hs2y);
	}
}
