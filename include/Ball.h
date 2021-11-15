#pragma once

//#include <SFML/Graphics/Sprite.hpp>

#include "Paddle.h"

// Forward declaration
namespace sf
{
	class Time;
}

namespace Pangu
{
	class Ball
	{
	public:
		Ball(const TextureHolder& textures);
		~Ball();

		void update(float dt);
		void checkArenaCollision(const sf::FloatRect& bounds);
		void checkPaddleCollision(Paddle player1, Paddle player2);

		int isScore(const sf::FloatRect& bounds);

		void setVelocity(sf::Vector2f velocity);
		void setVelocity(float vx, float vy);

		const sf::Vector2f getVelocity() const;

		sf::Sprite* getSprite();

		sf::FloatRect getPosition();

	private:
		static bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
								 float p2x, float p2y, float hs2x, float hs2y);

	private:
		sf::Sprite m_Sprite;
		float m_Radius;

		sf::Vector2f m_Velocity;
	};
}
