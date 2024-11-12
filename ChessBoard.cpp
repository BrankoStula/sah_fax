#include "ChessBoard.hpp"

ChessBoard::ChessBoard(){
    loadTextures();
    initializeBoard();
    initializeBoardSquares();
}

int ChessBoard::getBoardSize(){
    return boardSize;
}

int ChessBoard::getTileSize(){
    return tileSize;
}

void ChessBoard::loadTextures(){
    std::string pieces[] = {"king","queen","rook","bishop","knight","pawn"};
    std::string colors[] = {"white","black"};
    for(const auto& piece: pieces){
        for(const auto& color : colors){
            std::string path = "assets/" + color + "_" + piece + ".png";
            textures[color + "_" + piece].loadFromFile(path);
        }
    }
}

void ChessBoard::initializeBoard(){
    Piece initialLayout[boardSize][boardSize] = {{ {ROOK, BLACK}, {KNIGHT, BLACK}, {BISHOP, BLACK}, {QUEEN, BLACK}, {KING, BLACK}, {BISHOP, BLACK}, {KNIGHT, BLACK}, {ROOK, BLACK} },
        { {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK} },
        { {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE} },
        { {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE} },
        { {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE} },
        { {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE} },
        { {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE} },
        { {ROOK, WHITE}, {KNIGHT, WHITE}, {BISHOP, WHITE}, {QUEEN, WHITE}, {KING, WHITE}, {BISHOP, WHITE}, {KNIGHT, WHITE}, {ROOK, WHITE} }};


    for(int i = 0 ;i <boardSize;++i){
        for(int j = 0;j < boardSize; ++j){
            board[i][j] = initialLayout[i][j];
            if(board[i][j].type != EMPTY) {
                std::string pieceName = (board[i][j].color == WHITE ? "white_" : "black_") + pieceTypeName(board[i][j].type);
                board[i][j].setSprite(textures[pieceName]);
                board[i][j].sprite.setPosition(j * tileSize,i * tileSize);
            }
        }
    }
}

void ChessBoard::initializeBoardSquares() {
    for(int i=0; i < boardSize; ++i){
        for(int j =  0; j < boardSize; ++j){
            boardSquares[i][j].setSize(sf::Vector2f(tileSize,tileSize));
            boardSquares[i][j].setPosition(i* tileSize, j * tileSize);
            boardSquares[i][j].setFillColor((i + j) % 2 == 0 ? sf::Color(235,235,208) : sf::Color(119,148,85));

        }
    }
} 

std::string ChessBoard::pieceTypeName(PieceType type){
    switch(type) {
        case KING: return "king";
        case QUEEN: return "queen";
        case ROOK: return "rook";
        case BISHOP: return "bishop";
        case KNIGHT: return "knight";
        case PAWN: return "pawn";
        default: return ""; 
    }
}

void ChessBoard::draw(sf::RenderWindow& window) {
    for(int i = 0;i < boardSize; ++i){
        for (int j = 0;j < boardSize; ++j){
            window.draw(boardSquares[i][j]);
        }
    }

    for(int i = 0; i < boardSize; ++i){
        for(int j = 0; j < boardSize; ++j){
            if(board[i][j].type != EMPTY) {
                window.draw(board[i][j].sprite);
            } 
        }
    }
}