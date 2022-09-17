#include <SFML/Graphics.hpp>

class Paddle
{
	private:
	
	    // A RectangleShape object called ref
    	sf::RectangleShape paddleShape;
	
	public:
		Paddle(sf::Vector2f startPosition);

		float speed = 5; // can be changed for AI difficulty

		// The top-left pixel of the paddle
		sf::Vector2f position;
		sf::Vector2f size = sf::Vector2f(15, 150);
		
		void moveUp();
		void moveDown();
		void moveTowards(sf::Vector2f target);
		sf::Vector2f getCenter();

		void setColor(sf::Color);
		sf::RectangleShape getShape();
};