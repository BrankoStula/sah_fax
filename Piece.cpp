#include "Piece.hpp"
#include <iostream>

Piece::Piece(PieceType type, Color color):
    type(type),color(color) {}

void Piece::setSprite(const sf::Texture &texture){
   sprite.setTexture(texture);
}

bool Piece::isValidMove(int startX, int startY, int endX, int endY, Piece board[][8],std::pair<int, int> enPassantTarget) {
    bool valid = false;

    if (type == PAWN) {
        int direction = (color == WHITE) ? -1 : 1;  // White moves up (-1), Black down (+1)

        // Regular move (1 square forward)
        if (startX == endX && endY == startY + direction && board[endX][endY].type == EMPTY) {
            valid = true;
        }

        // First move (2 squares forward)
        if (startX == endX && endY == startY + 2 * direction &&
            ((color == WHITE && startY == 6) || (color == BLACK && startY == 1)) &&
            board[endX][endY].type == EMPTY && board[startX][startY + direction].type == EMPTY) {
            valid = true;
        }

        // Diagonal capture
        if (abs(startX - endX) == 1 && endY == startY + direction &&
            board[endX][endY].type != EMPTY && board[endX][endY].color != color) {
            valid = true;
        }

                // En Passant (special capture move)
        if (abs(startX - endX) == 1 && endY == startY + direction &&
            board[endX][endY].type == EMPTY && // Target square is empty
            endX == enPassantTarget.first && endY == enPassantTarget.second) {
            valid = true; // En passant move is valid
        }
    }

    if (type == KNIGHT) {
        int dx = abs(endX - startX);
        int dy = abs(endY - startY);

        if((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            if(board[endX][endY].type == EMPTY || board[endX][endY].color != color) {
                valid = true;
            }
        }
    }
    
    if (type == BISHOP) {
    // Check if the move is along a diagonal
    if (abs(startX - endX) == abs(startY - endY)) {
        int stepX = (endX > startX) ? 1 : -1;  // Direction of movement in X
        int stepY = (endY > startY) ? 1 : -1;  // Direction of movement in Y

        int currentX = startX + stepX;
        int currentY = startY + stepY;

        // Check each square along the diagonal path
        while (currentX != endX && currentY != endY) {
            // If any square along the path is not empty, the move is invalid
            if (board[currentX][currentY].type != EMPTY) {
                return false;
            }
            currentX += stepX;
            currentY += stepY;
        }

        // Ensure the destination square is either empty or occupied by an opponent
        if (board[endX][endY].type == EMPTY || board[endX][endY].color != color) {
            valid = true;
        }
    }
}

    if (type == QUEEN) {
    // The Queen combines the movement of the Rook and the Bishop.
    
    // Check diagonal moves (Bishop-like)
    if (abs(startX - endX) == abs(startY - endY)) {
        int dx = (endX > startX) ? 1 : -1;
        int dy = (endY > startY) ? 1 : -1;

        int x = startX + dx, y = startY + dy;
        while (x != endX && y != endY) {
            if (board[x][y].type != EMPTY) {
                return false; // A piece blocks the path
            }
            x += dx;
            y += dy;
        }
        valid = (board[endX][endY].type == EMPTY || board[endX][endY].color != color); // Valid if empty or opponent
    }

    // Check straight moves (Rook-like)
    if (startX == endX || startY == endY) {
        int dx = (startX == endX) ? 0 : (endX > startX ? 1 : -1);
        int dy = (startY == endY) ? 0 : (endY > startY ? 1 : -1);

        int x = startX + dx, y = startY + dy;
        while (x != endX || y != endY) {
            if (board[x][y].type != EMPTY) {
                return false; // A piece blocks the path
            }
            x += dx;
            y += dy;
        }
        valid = (board[endX][endY].type == EMPTY || board[endX][endY].color != color); // Valid if empty or opponent
    }
}

    if (type == ROOK) {
        // Rook can move horizontally or vertically
        if (startX == endX) {  // Vertical move
            int step = (endY > startY) ? 1 : -1;
            for (int y = startY + step; y != endY; y += step) {
                if (board[startX][y].type != EMPTY) {
                    return false;  // Blocked by another piece
                }
            }
        } else if (startY == endY) {  // Horizontal move
            int step = (endX > startX) ? 1 : -1;
            for (int x = startX + step; x != endX; x += step) {
                if (board[x][startY].type != EMPTY) {
                    return false;  // Blocked by another piece
                }
            }
        } else {
            return false;  // Invalid move for Rook (it can only move in straight lines)
        }

        // Check if the destination is either empty or occupied by an opponent's piece
        if (board[endX][endY].type != EMPTY && board[endX][endY].color == color) {
            return false;  // Can't capture own piece
        }
        board[startX][startY].hasMoved = true;
        valid = true;
    }

    if (type == KING) {
        // The King can move one square in any direction (vertically, horizontally, or diagonally)
        if (abs(startX - endX) <= 1 && abs(startY - endY) <= 1) {
            // Ensure the target square is either empty or occupied by an opponent's piece
            if ((board[endX][endY].type == EMPTY || board[endX][endY].color != color) &&
                !isKingInCheck(startX, startY, endX, endY, board,enPassantTarget)) {
                board[startX][startY].hasMoved = true;
                valid = true; // Valid move
            }
        }

        if(!hasMoved && startY == endY && abs(startX - endX) == 2) {
            int rookX = (endX > startX) ? 7 : 0;
            int direction = (endX > startX) ? 1 : -1;

            for(int x = startX + direction;x != rookX; x+= direction){
                if(board[x][startY].type != EMPTY){
                    return false;
                }
            }

            if(board[rookX][startY].type == ROOK && !board[rookX][startY].hasMoved){
                for(int x = startX; x!= endX + direction; x+= direction){
                    if(isKingInCheck(startX, startY, x, startY, board,enPassantTarget)){
                        return false;
                    }
                }
                board[startX][startY].hasMoved = true;
                valid = true;
            }
        }
    }

    if(valid) {
        if(!isMoveValidUnderCheck(startX,startY,endX,endY,board,enPassantTarget)) {
            return false;
        }
    }

    return valid;
}


bool Piece::isKingInCheck(int startX, int startY, int endX, int endY, Piece board[][8],std::pair<int, int> enPassantTarget) {
    // Temporarily move the king to the target position
    Piece originalPiece = board[endX][endY]; 
    board[endX][endY] = board[startX][startY]; 
    board[startX][startY].type = EMPTY;


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].type != EMPTY && board[i][j].color != board[endX][endY].color) {
                if (board[i][j].isValidMove(i, j, endX, endY, board,enPassantTarget)) {
                    // Revert the move before returning
                    board[startX][startY] = board[endX][endY];
                    board[endX][endY] = originalPiece;
                    return true;
                }
            }
            if (board[i][j].type == PAWN && board[i][j].color != board[endX][endY].color) {
                // Pawns attack diagonally
                if ((board[i][j].color == WHITE && i == endX - 1 && (j == endY - 1 || j == endY + 1)) ||  // White pawn
                    (board[i][j].color == BLACK && i == endX + 1 && (j == endY - 1 || j == endY + 1))) {  // Black pawn
                    // Revert the move before returning
                    board[startX][startY] = board[endX][endY];
                    board[endX][endY] = originalPiece;
                    return true;
                }
            }
        }
    }

        // Ensure the opposing king is not adjacent
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int adjacentX = endX + dx;
            int adjacentY = endY + dy;

            // Skip out-of-bounds and the king's own square
            if (adjacentX < 0 || adjacentX >= 8 || adjacentY < 0 || adjacentY >= 8 || (dx == 0 && dy == 0)) {
                continue;
            }

            // Check if there's an opposing king in the adjacent square
            if (board[adjacentX][adjacentY].type == KING && board[adjacentX][adjacentY].color != board[endX][endY].color) {
                // Revert the move before returning
                board[startX][startY] = board[endX][endY];
                board[endX][endY] = originalPiece;
                return true;
            }
        }
    }

    // Revert the move
    board[startX][startY] = board[endX][endY];
    board[endX][endY] = originalPiece;

    return false;
}


bool Piece::isMoveValidUnderCheck(int startX, int startY, int endX, int endY, Piece board[][8],std::pair<int, int> enPassantTarget) {
    // Temporarily make the move
    Piece originalPiece = board[endX][endY];       // Save the piece at the target position
    board[endX][endY] = board[startX][startY];    // Move the piece to the target position
    board[startX][startY].type = EMPTY;           // Mark the original position as empty

    // Find the King of the current player
    bool kingInCheck = false;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].type == KING && board[i][j].color == board[endX][endY].color) {
                // Check if this King is in check after the move
                kingInCheck = board[i][j].isKingInCheck(i, j, i, j, board,enPassantTarget);
                break;
            }
        }
        if (kingInCheck) break;
    }

    // Revert the move
    board[startX][startY] = board[endX][endY];    // Return the piece to its original position
    board[endX][endY] = originalPiece;           // Restore the original piece (if any)

    // Return true if the move resolves the check
    return !kingInCheck;
}
