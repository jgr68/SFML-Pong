#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include "scoreboard.h"

#define TITLEBAR "SFML-Pong!"
#define PTS_2_WIN 10
#define FONT_PATH "resources/DejaVuSans.ttf"


// prototypes
void processInput(sf::RectangleShape&, int, bool, bool, int, int);
void setBallVel(int&, int&, int, int);
void moveBall(sf::RectangleShape&, int&, int&, int, int, sf::RectangleShape, sf::RectangleShape, scoreboard&);

int main()
{
    // SET UP! ----------------------------------------------------------------

    // determine the window dimensions based on the screen resolution
    unsigned int windowH = sf::VideoMode::getDesktopMode().height * 2 / 3;
    unsigned int windowW = sf::VideoMode::getDesktopMode().width * 2 / 3;

    // use the window size to determine the paddle dimensions and velocity
    unsigned int paddleH = windowH / 5;
    unsigned int paddleW = windowW / 38;
    int paddleDY = windowH / 42;

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

    // load font
    sf::Font font;
    if (!font.loadFromFile(FONT_PATH))
    {
        std::cerr << "Unable to load font. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }

    // make a scoreboard
    scoreboard mySB(PTS_2_WIN);
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setColor(sf::Color::White);
    // haven't seemed to figure out sfml's text rendering stuff yet...
    scoreText.setPosition(windowW / 8h, windowH / 16);

    // LET'S PLAY! ------------------------------------------------------------
	// start the game loop
    while (app.isOpen())
    {
        // update the score board
        scoreText.setString(mySB.printScoreBoard());

        // move the ball
        moveBall(ball, dx, dy, windowH, windowW, lPaddle, rPaddle, mySB);

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
        app.draw(scoreText);

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
        dx = windowW / 202 * (rand() % 2 - 1);
    } while(dx == 0);

    do {
        dy = windowH / 144 * (rand() % 2 - 1);
    } while(dy == 0);
}

void moveBall(sf::RectangleShape& ball, int& dx, int& dy, int winH, int winW, sf::RectangleShape lPaddle, sf::RectangleShape rPaddle, scoreboard& mySB)
{
    if (dx < 0 && ball.getPosition().x < 0)
    {
        mySB.bumpRScore();
        ball.setPosition(winW / 2, winH / 2);
    }

    if (dy > 0 && ball.getPosition().x + ball.getSize().x + dx > winW)
    {
        mySB.bumpLScore();
        ball.setPosition(winW / 2, winH / 2);
    }

    // COLLISION DETECTION
    if (dx < 0 && ball.getGlobalBounds().intersects(lPaddle.getGlobalBounds()))
        dx *= -1;

    if (dx > 0 && ball.getGlobalBounds().intersects(rPaddle.getGlobalBounds()))
        dx *= -1;

    // make ball bounce off of horizontal walls
    if (dy < 0 && ball.getPosition().y + dy < 0)
        dy *= -1;
    if (dy > 0 && ball.getPosition().y + ball.getSize().y + dy > winH)
        dy *= -1;

    // move the ball
    ball.move(dx, dy);
}
