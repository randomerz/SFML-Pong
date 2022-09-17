#include <SFML/Graphics.hpp>

class Ball
{
	private:
	
	    // A RectangleShape object called ref
    	sf::RectangleShape ballShape;
	
	public:
		Ball(sf::Vector2f startPosition, sf::Vector2f startVelocity);

		// The top-left pixel of the square 'ball'
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Vector2f size = sf::Vector2f(20, 20);

		void updateMovement();
		sf::Vector2f getCenter();

		sf::RectangleShape getShape();
};