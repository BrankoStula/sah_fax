#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>

enum PieceType {KING,QUEEN,ROOK,BISHOP,KNIGHT,PAWN,EMPTY};
enum Color {WHITE, BLACK, NONE};

class Piece{
    public:
        PieceType type;
        Color color;
        sf::Sprite sprite;
        bool hasMoved = false;

        Piece(PieceType type=EMPTY, Color color = NONE);

        void setSprite(const sf::Texture &texture);
        bool isValidMove(int startX,int startY,int endX, int endY, Piece board[][8],std::pair<int, int> enPassantTarget);
        bool isKingInCheck(int startX, int startY,int endX,int endY, Piece board[][8],std::pair<int, int> enPassantTarget);
        bool isMoveValidUnderCheck(int startX, int startY, int endX, int endY, Piece board[][8],std::pair<int, int> enPassantTarget);
};

#endif