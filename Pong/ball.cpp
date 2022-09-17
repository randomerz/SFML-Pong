#include "ball.h"

Ball::Ball(sf::Vector2f initPosition, sf::Vector2f initVelocity)
{
    position = initPosition - (size / 2.0f);
    velocity = initVelocity;
 
    ballShape.setSize(size);
    ballShape.setPosition(position);
}

void Ball::updateMovement()
{
    position += velocity;
    ballShape.setPosition(position);
}

sf::Vector2f Ball::getCenter()
{
    return position + (size / 2.0f);
}

sf::RectangleShape Ball::getShape()
{
    return ballShape;
}