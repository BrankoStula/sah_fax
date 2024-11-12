#include <SFML/Graphics.hpp>
#include "ChessBoard.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(ChessBoard::getBoardSize()  * ChessBoard::getTileSize(),ChessBoard::getBoardSize() * ChessBoard::getTileSize()), "Chess Game");
    ChessBoard chessBoard;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        
    }

    window.clear(sf::Color::White);
    chessBoard.draw(window);
    window.display();
    }

    return 0;
}

