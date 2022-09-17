#include "ball.h"
#include "constants.h"
#include "paddle.h"
#include "vectorMath.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>

sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;
sf::Text playerScoreText;
sf::Text aiScoreText;
sf::Font font;

int playerScore;
int aiScore;

void initBackgroundSprite();
void initHud();
void updateScore();
void resetBall(Ball* ball);
bool doesBallPaddleOverlap(Ball* ball, Paddle* paddle);
void doPaddleHit(Ball* ball);

int main()
{
    const unsigned int FPS = 60; //The desired FPS. (The number of updates each second).

    srand (time(NULL));


    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong!");
    window.setFramerateLimit(FPS);  
    initBackgroundSprite();
    initHud();

    Ball ball(
        sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
        sf::Vector2f(4, 4)
    );
    resetBall(&ball);

    Paddle player(sf::Vector2f(WINDOW_WIDTH / 15, WINDOW_HEIGHT / 2));
    player.setColor(sf::Color(255, 0, 255));

    Paddle aiPaddle(sf::Vector2f(WINDOW_WIDTH * 14.f / 15, WINDOW_HEIGHT / 2));
    aiPaddle.speed = 2.5f;
    aiPaddle.setColor(sf::Color(255, 255, 0));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //////////////////////////////
        // Check inputs
        //////////////////////////////

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            player.moveUp();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            player.moveDown();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            std::cout << "Quiting Application..." << "\n";
            window.close();
            break;
        }

        aiPaddle.moveTowards(ball.position);

        //////////////////////////////
        // Do physics
        //////////////////////////////

        // Move ball
        ball.updateMovement();

        // Check collisions
        if (ball.position.y <= 0) 
        {
            ball.position.y = 0;
            ball.velocity.y *= -1;
        }
        else if (ball.position.y >= WINDOW_HEIGHT - ball.size.y)
        {
            ball.position.y = WINDOW_HEIGHT - ball.size.y;
            ball.velocity.y *= -1;
        }

        // Out of bounds
        if (ball.position.x <= 0) 
        {
            // ball.position.x = 0;
            // ball.velocity.x *= -1;
            aiScore += 1;
            updateScore();
            resetBall(&ball);
        }
        else if (ball.position.x >= WINDOW_WIDTH - ball.size.x)
        {
            // ball.position.x = WINDOW_WIDTH - ball.size.x;
            // ball.velocity.x *= -1;
            playerScore += 1;
            updateScore();
            resetBall(&ball);
        }

        // Paddle collisions
        if (doesBallPaddleOverlap(&ball, &player)) 
        {
            ball.position.x = player.position.x + player.size.x;
            sf::Vector2f dir = normalize(
                ball.getCenter() - player.getCenter() + sf::Vector2f(50, 0)
            );
            ball.velocity = dir * magnitude(ball.velocity);
            doPaddleHit(&ball);
        }
        else if (doesBallPaddleOverlap(&ball, &aiPaddle))
        {
            ball.position.x = aiPaddle.position.x - ball.size.x;
            sf::Vector2f dir = normalize(
                ball.getCenter() - aiPaddle.getCenter() - sf::Vector2f(50, 0)
            );
            ball.velocity = dir * magnitude(ball.velocity);
            doPaddleHit(&ball);
        }


        //////////////////////////////
        // Draw screen
        //////////////////////////////

        window.clear();
        window.draw(backgroundSprite);
        updateScore();
        window.draw(playerScoreText);
        window.draw(aiScoreText);
        window.draw(player.getShape());
        window.draw(aiPaddle.getShape());
        window.draw(ball.getShape());
        window.display();
    }

    return 0;
}

void initBackgroundSprite()
{
    if (!backgroundTexture.loadFromFile("../Pong/textures/game_background.png", sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)))
    {
        std::cout << "Couldn't load background texture.\n";
    }
    backgroundSprite.setTexture(backgroundTexture);

    sf::Vector2u backgroundTextureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(
        sf::Vector2f(
            static_cast<float>(WINDOW_WIDTH) / backgroundTextureSize.x, 
            static_cast<float>(WINDOW_HEIGHT) / backgroundTextureSize.y
        )
    );
}

void initHud()
{
    if (!font.loadFromFile("../Pong/fonts/Roboto-Bold.ttf"))
    {
        std::cout << "Couldn't load font.\n";
    }
    playerScoreText.setFont(font);
    playerScoreText.setCharacterSize(120);
    playerScoreText.setFillColor(sf::Color(128, 128, 196));
    aiScoreText.setFont(font);
    aiScoreText.setCharacterSize(120);
    aiScoreText.setFillColor(sf::Color(128, 128, 196));

    sf::FloatRect playerTextRect = playerScoreText.getLocalBounds();
    playerScoreText.setOrigin(playerTextRect.left + playerTextRect.width / 2.0f,
                              playerTextRect.top + playerTextRect.height / 2.0f);
    sf::FloatRect aiTextRect = aiScoreText.getLocalBounds();
    aiScoreText.setOrigin(aiTextRect.left + aiTextRect.width / 2.0f,
                          aiTextRect.top + aiTextRect.height / 2.0f);
    playerScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT * 2.0f / 5.0f));
    aiScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH * 3.0f / 4.0f, WINDOW_HEIGHT * 2.0f / 5.0f));

    updateScore();
}

void updateScore()
{
    playerScoreText.setString(std::to_string(playerScore));
    aiScoreText.setString(std::to_string(aiScore));
}

void resetBall(Ball* ball)
{
    ball->position = sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    int verticalMultiplier = ((rand() % 2) * 2) - 1;
    if (playerScore > aiScore)
        ball->velocity = sf::Vector2f(-4, verticalMultiplier * 4);
    else
        ball->velocity = sf::Vector2f(4, verticalMultiplier * 4);
}

bool doesBallPaddleOverlap(Ball* ball, Paddle* paddle)
{
    sf::Vector2f bp = ball->position;
    sf::Vector2f bs = ball->size;
    sf::Vector2f pp = paddle->position;
    sf::Vector2f ps = paddle->size;

    // Checks if both ranges overlap
    return bp.x <= pp.x + ps.x &&
           pp.x <= bp.x + bs.x &&
           bp.y <= pp.y + ps.y &&
           pp.y <= bp.y + bs.y;
}

void doPaddleHit(Ball* ball)
{
    ball->velocity = ball->velocity * 1.1f;
}



