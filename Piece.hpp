#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>

enum PieceType {KING,QUEEN,ROOK,BISHOP,KNIGHT, PAWN,EMPTY};
enum Color {WHITE, BLACK, NONE};

class Piece{
    public:
        PieceType type;
        Color color;
        sf::Sprite sprite;

        Piece(PieceType type=EMPTY, Color color = NONE);

        void setSprite(const sf::Texture &texture);

};

#endif