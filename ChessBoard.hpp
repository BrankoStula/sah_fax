#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "Piece.hpp"
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

enum Turn {WHITE_TURN, BLACK_TURN};

class ChessBoard{
    private:
    static const int boardSize = 8;
    static const int tileSize = 80;

    Piece board[boardSize][boardSize];
    sf::RectangleShape boardSquares[boardSize][boardSize];
    std::map<std::string,sf::Texture> textures;

    Piece* selectedPiece = nullptr;
    Turn currentTurn;
    std::pair<int,int> enPassantTarget = {-1,-1};
    

    public:

    ChessBoard();

    //Initializaion
    void loadTextures();
    void initializeBoard();
    void initializeBoardSquares();
    std::string pieceTypeName(PieceType type);
    void draw(sf::RenderWindow &window);

    //Getters
    static int getBoardSize();
    static int getTileSize(); 

    //Player inputs
    void handleMouseInput(sf::RenderWindow& window);
    void highlightAvailableMoves(int x,int y);
    void movePiece(Piece* piece, int x, int y);
    void nextTurn();
};


#endif