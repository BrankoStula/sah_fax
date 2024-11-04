#include <SFML/Graphics.hpp>
#include <iostream>

const int boardSize = 8;
const int tileSize = 80; // Size of each square in pixels

int main()
{
    sf::RenderWindow window(sf::VideoMode(boardSize * tileSize, boardSize * tileSize), "Chess Game");

    // Load textures (for simplicity, we will just color squares here)
    sf::RectangleShape board[boardSize][boardSize];

    // Initialize the chessboard grid
    for (int i = 0; i < boardSize; ++i)
    {
        for (int j = 0; j < boardSize; ++j)
        {
            board[i][j].setSize(sf::Vector2f(tileSize, tileSize));
            board[i][j].setPosition(i * tileSize, j * tileSize);

            // Alternate colors for a chessboard pattern
            if ((i + j) % 2 == 0)
                board[i][j].setFillColor(sf::Color(235, 235, 208)); // Light color
            else
                board[i][j].setFillColor(sf::Color(119, 148, 85)); // Dark color
        }
    }

    // Main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw the board
        for (int i = 0; i < boardSize; ++i)
        {
            for (int j = 0; j < boardSize; ++j)
            {
                window.draw(board[i][j]);
            }
        }

        window.display();
    }

    return 0;
}
