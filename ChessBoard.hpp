#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "Piece.hpp"
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class ChessBoard{
    private:
    static const int boardSize = 8;
    static const int tileSize = 80;

    Piece board[boardSize][boardSize];
    sf::RectangleShape boardSquares[boardSize][boardSize];
    std::map<std::string,sf::Texture> textures;

    public:
    ChessBoard();
    void loadTextures();
    void initializeBoard();
    void initializeBoardSquares();
    std::string pieceTypeName(PieceType type);
    void draw(sf::RenderWindow &window);

    static int getBoardSize();
    static int getTileSize(); 
};


#endif