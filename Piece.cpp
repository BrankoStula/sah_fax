#include "Piece.hpp"

Piece::Piece(PieceType type, Color color):
    type(type),color(color) {}

void Piece::setSprite(const sf::Texture &texture){
   sprite.setTexture(texture);
}