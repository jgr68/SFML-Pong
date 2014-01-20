#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>

// screen attributes
#define TITLEBAR "SFML-Pong!"

// prototypes
void processInput(sf::RectangleShape&, int, bool, bool, int, int);
void setBallVel(int&, int&, int, int);
void moveBall(sf::RectangleShape&, int&, int&, int, int);

int main()
{
    // SET UP! ----------------------------------------------------------------
    // determine the window dimensions based on the screen resolution
    unsigned int windowH = sf::VideoMode::getDesktopMode().height * 2 / 3;
    unsigned int windowW = sf::VideoMode::getDesktopMode().width * 2 / 3;

    // use the window size to determine the paddle dimensions and velocity
    unsigned int paddleH = windowH / 5;
    unsigned int paddleW = windowW / 38;
    int paddleDY = 6;

    // use the paddle dimensions to instantiate the ball
    sf::RectangleShape ball(sf::Vector2f(paddleW / 2, paddleW / 2));
    ball.setPosition(windowW / 2 - paddleW / 2, windowH / 2 - paddleW / 2);
    int dx, dy;
    dx = dy = 0;
    setBallVel(dx, dy, windowH, windowW);

    // instantiate main window
    sf::RenderWindow app(sf::VideoMode(windowW, windowH), TITLEBAR, sf::Style::Titlebar);

    // instantiate the paddles and give them starting positions
    sf::RectangleShape lPaddle(sf::Vector2f(paddleW, paddleH));
    sf::RectangleShape rPaddle(sf::Vector2f(paddleW, paddleH));
    lPaddle.setPosition(0, windowH / 2 - paddleH / 2);
    rPaddle.setPosition(windowW - paddleW, windowH / 2 - paddleH / 2);

    // LET'S PLAY! ------------------------------------------------------------
	// start the game loop
    while (app.isOpen())
    {
        // move the ball
        moveBall(ball, dx, dy, windowH, windowW);

        // process realtime inputs
        processInput(lPaddle, paddleDY, sf::Keyboard::isKeyPressed(sf::Keyboard::Tab), sf::Keyboard::isKeyPressed(sf::Keyboard::LShift), windowH, paddleH);
        processInput(rPaddle, paddleDY, sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash), sf::Keyboard::isKeyPressed(sf::Keyboard::RShift), windowH, paddleH);

        // process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // handle events
            if (event.type == sf::Event::Closed)
                app.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                app.close();
        }

        // clear the screen
        app.clear();

        // draw all entities
        app.draw(ball);
        app.draw(lPaddle);
        app.draw(rPaddle);
        // update the window
        app.display();
    }

    return EXIT_SUCCESS;
}


void processInput(sf::RectangleShape& paddle, int dy, bool goUp, bool goDown, int windowH, int paddleH)
{
    // handle realtime input
    if (goUp)
    {
        if (paddle.getPosition().y <= 0)
            paddle.setPosition(paddle.getPosition().x, 0);
        else
            paddle.move(0, -1 * dy);
    }
    if (goDown)
    {
        if (paddle.getPosition().y + paddleH >= windowH)
            paddle.setPosition(paddle.getPosition().x, windowH - paddleH);
        else
            paddle.move(0, dy);
    }
}

void setBallVel(int& dx, int& dy, int windowH, int windowW)
{
    srand(time(NULL));
    do {
        dx = windowW / 72 * (rand() % 2 - 1);
    } while(dx == 0);

    do {
        dy = windowH / 72 * (rand() % 2 - 1);
    } while(dy == 0);
}

void moveBall(sf::RectangleShape& ball, int& dx, int& dy, int winH, int winW)
{
    // COLLISION DETECTION
    // temporary ... remove after calibrating ball velocity
    if (dx < 0 && ball.getPosition().x + dx < 0)
        dx *= -1;
    if (dx > 0 && ball.getPosition().x + ball.getSize().x + dx > winW)
        dx *= -1;

    // make ball bounce off of horizontal walls
    if (dy < 0 && ball.getPosition().y + dy < 0)
        dy *= -1;
    if (dy > 0 && ball.getPosition().y + ball.getSize().y + dy > winH)
        dy *= -1;

    // move the ball
    ball.move(dx, dy);
}
