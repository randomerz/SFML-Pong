#include "vectorMath.h"

#include <math.h>

float magnitude(sf::Vector2f v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(sf::Vector2f v)
{
	return v / magnitude(v);
}