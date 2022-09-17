#include "constants.h"
#include "paddle.h"
 
Paddle::Paddle(sf::Vector2f initPosition)
{
    position = initPosition - (size / 2.0f);
 
    paddleShape.setSize(size);
    paddleShape.setPosition(position);
}

void Paddle::moveUp()
{
    // Top of screen is y = 0
    position.y = std::max(0.f, position.y - speed);
    paddleShape.setPosition(position);
}

void Paddle::moveDown()
{
    position.y = std::min(WINDOW_HEIGHT - size.y, position.y + speed);
    paddleShape.setPosition(position);
}

void Paddle::moveTowards(sf::Vector2f target)
{
    if (target.y < position.y + (size.y / 2.0f))
    {
        moveUp();
    }
    else
    {
        moveDown();
    }
}

sf::Vector2f Paddle::getCenter()
{
    return position + (size / 2.0f);
}

void Paddle::setColor(sf::Color color)
{
    paddleShape.setFillColor(color);
}

sf::RectangleShape Paddle::getShape()
{
    return paddleShape;
}