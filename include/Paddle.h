#pragma once

#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

// Forward declaration
namespace sf
{
	class Time;
}

namespace Pangu
{
	class Paddle
	{
	public:
		Paddle(const TextureHolder& textures);

		void update(const float& dt);
		void checkArenaCollision(const sf::FloatRect& bounds);

		inline void setAI(bool isAI) { m_AI = isAI; }
		const bool isAI() const { return m_AI; };

		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);
		void setAcceleration(const float& acceleration);

		const sf::Vector2f getVelocity() const;

		const float getAcceleration() const;
		float* getAccelerationPtr();

		sf::Sprite* getSprite();

		sf::FloatRect getPosition();

	private:
		sf::Sprite m_Sprite;
		bool m_AI;

		sf::Vector2f m_Velocity;
		float m_Acceleration;
	};
}
