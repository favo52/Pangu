#include <Ball.h>

#include <iostream>

pangu::Ball::Ball()
{
	m_ball.setRadius(15.0f);
	m_ball.setFillColor(sf::Color::Green);
	centerOrigin(m_ball);
}

pangu::Ball::~Ball()
{
}

void pangu::Ball::centerOrigin(sf::CircleShape& circle)
{
	sf::FloatRect titleRect = circle.getLocalBounds();
	circle.setOrigin(titleRect.left + circle.getRadius(), titleRect.top + circle.getRadius());
}
