#include "ChessBoard.hpp"
#include <iostream>

ChessBoard::ChessBoard(): currentTurn(WHITE_TURN){
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

void ChessBoard::initializeBoard() {
    // Define the initial layout directly according to the desired rows
    Piece initialLayout[boardSize][boardSize] = {
        {{ROOK, BLACK}, {KNIGHT, BLACK}, {BISHOP, BLACK}, {QUEEN, BLACK}, {KING, BLACK}, {BISHOP, BLACK}, {KNIGHT, BLACK}, {ROOK, BLACK}}, // Black pieces (Row 0)
        {{PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}, {PAWN, BLACK}},       // Black pawns (Row 1)
        {{EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}},       // Empty row (Row 2)
        {{EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}},       // Empty row (Row 3)
        {{EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}},       // Empty row (Row 4)
        {{EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}, {EMPTY, NONE}},       // Empty row (Row 5)
        {{PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}, {PAWN, WHITE}},       // White pawns (Row 6)
        {{ROOK, WHITE}, {KNIGHT, WHITE}, {BISHOP, WHITE}, {QUEEN, WHITE}, {KING, WHITE}, {BISHOP, WHITE}, {KNIGHT, WHITE}, {ROOK, WHITE}} // White pieces (Row 7)
    };

 
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            board[i][j] = initialLayout[j][i];

            // Set sprites for non-empty pieces
            if (board[i][j].type != EMPTY) {
                std::string pieceName = (board[i][j].color == WHITE ? "white_" : "black_") + pieceTypeName(board[i][j].type);
                board[i][j].setSprite(textures[pieceName]);
                board[i][j].sprite.setPosition(i * tileSize, j*tileSize);
            }
        }
    }
}

void ChessBoard::initializeBoardSquares() {
    // Initialize the squares of the board
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            boardSquares[i][j].setSize(sf::Vector2f(tileSize, tileSize));
            boardSquares[i][j].setPosition(i * tileSize, j* tileSize);
            boardSquares[i][j].setFillColor((i + j) % 2 == 0 ? sf::Color(235, 235, 208) : sf::Color(119, 148, 85));
        }
    }
}

void ChessBoard::handleMouseInput(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    //What Square Was Clicked On
    int x = mousePos.x / tileSize;
    int y = mousePos.y / tileSize; 

    Color currentPlayerColor =  (currentTurn == WHITE_TURN) ? WHITE : BLACK;

    //std::cout << "Current Player: " << (currentPlayerColor == WHITE ? "White" : "Black") << std::endl;
    //std::cout << "Clicked on Tile (" << x << ", " << y << ") Type: " << board[x][y].type << " Color: " << board[x][y].color << std::endl;

    if(board[x][y].type != EMPTY && board[x][y].color == currentPlayerColor){
        selectedPiece = &board[x][y];
        //std::cout << "Selected Piece at (" << x << ", " << y << ")" << std::endl;
        highlightAvailableMoves(x,y);
    }else if(selectedPiece) 
    {
        //std::cout << "x:" <<selectedPiece->sprite.getPosition().x / tileSize <<"Y:"<< selectedPiece->sprite.getPosition().y / tileSize << std::endl;
        if(selectedPiece->isValidMove(selectedPiece->sprite.getPosition().x / tileSize, selectedPiece->sprite.getPosition().y / tileSize, x, y, board,enPassantTarget))
        {
            movePiece(selectedPiece,x,y);
            nextTurn();
        }
        selectedPiece = nullptr;
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
    // Draw squares first
    for(int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            window.draw(boardSquares[i][j]);
        }
    }

    // Draw pieces on top of squares
    for(int i = 0; i < boardSize; ++i) {
        for(int j = 0; j < boardSize; ++j) {
            if(board[i][j].type != EMPTY) {
                window.draw(board[i][j].sprite);
            } 
        }
    }
}

void ChessBoard::highlightAvailableMoves(int x, int y)
{
    // Reset all squares to their original colors
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            boardSquares[i][j].setFillColor((i + j) % 2 == 0 ? sf::Color(235, 235, 208) : sf::Color(119, 148, 85));
        }
    }

    // Highlight moves for Pawn
    if (selectedPiece->type == PAWN) {
        int direction = (selectedPiece->color == WHITE) ? -1 : 1;

        // Highlight move direction (forward)
        if (y + direction >= 0 && y + direction < boardSize && board[x][y + direction].type == EMPTY) {
            boardSquares[x][y + direction].setFillColor(sf::Color::Green);
        }

        // Highlight double move on initial pawn move
        if ((selectedPiece->color == WHITE && y == 6) || (selectedPiece->color == BLACK && y == 1)) {
            if (y + 2 * direction >= 0 && y + 2 * direction < boardSize && board[x][y + 2 * direction].type == EMPTY) {
                boardSquares[x][y + 2 * direction].setFillColor(sf::Color::Green);
            }
        }

        // Highlight captures
        if (x - 1 >= 0 && y + direction >= 0 && y + direction < boardSize && board[x - 1][y + direction].type != EMPTY && board[x - 1][y + direction].color != selectedPiece->color) {
            boardSquares[x - 1][y + direction].setFillColor(sf::Color::Red);
        }
        if (x + 1 < boardSize && y + direction >= 0 && y + direction < boardSize && board[x + 1][y + direction].type != EMPTY && board[x + 1][y + direction].color != selectedPiece->color) {
            boardSquares[x + 1][y + direction].setFillColor(sf::Color::Red);
        }
    }
    
    // Highlight moves for Knight
    if (selectedPiece->type == KNIGHT) {
        // Possible knight moves
        int knightMoves[8][2] = {
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
            {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
        };

        for (auto move : knightMoves) {
            int newX = x + move[0];
            int newY = y + move[1];

            // Check if the move is within bounds
            if (newX >= 0 && newX < boardSize && newY >= 0 && newY < boardSize) {
                // Highlight if the square is empty or contains an opponent's piece
                if (board[newX][newY].type == EMPTY) {
                    boardSquares[newX][newY].setFillColor(sf::Color::Green);
                } else if (board[newX][newY].color != selectedPiece->color) {
                    boardSquares[newX][newY].setFillColor(sf::Color::Red);
                }
            }
        }
    }

    // Highlight moves for Bishop
    if (selectedPiece->type == BISHOP) {
    int directions[4][2] = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // Diagonal directions
    };

    for (auto dir : directions) {
        int newX = x, newY = y;

        // Continue moving in the current direction until a boundary or piece is hit
        while (true) {
            newX += dir[0];
            newY += dir[1];

            // Stop if the position is out of bounds
            if (newX < 0 || newX >= boardSize || newY < 0 || newY >= boardSize) {
                break;
            }

            // If the square is empty, highlight it
            if (board[newX][newY].type == EMPTY) {
                boardSquares[newX][newY].setFillColor(sf::Color::Green);
            }
            // If the square contains a piece, highlight it if it's an opponent's piece, then stop
            else if (board[newX][newY].color != selectedPiece->color) {
                boardSquares[newX][newY].setFillColor(sf::Color::Red);
                break;
            }
            // Stop if the square contains a piece of the same color
            else {
                break;
            }
        }
    }
}

    // Highlight moves for Queen
    if (selectedPiece->type == QUEEN) {
    // The Queen combines the logic of the Rook and the Bishop, moving in all straight lines and diagonals.
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Skip the case where both dx and dy are 0 (no movement)
            if (dx == 0 && dy == 0) continue;

            int currentX = x + dx;
            int currentY = y + dy;

            // Traverse in the current direction until the board edge or a piece is hit
            while (currentX >= 0 && currentX < boardSize && currentY >= 0 && currentY < boardSize) {
                if (board[currentX][currentY].type == EMPTY) {
                    boardSquares[currentX][currentY].setFillColor(sf::Color::Green);
                } else {
                    if (board[currentX][currentY].color != selectedPiece->color) {
                        boardSquares[currentX][currentY].setFillColor(sf::Color::Red);
                    }
                    break; // Stop further movement in this direction if a piece is encountered
                }
                currentX += dx;
                currentY += dy;
            }
        }
    }
}

    // Highlight moves for Rook
    if (selectedPiece->type == ROOK) {
    // Horizontal moves to the right
    for (int i = x + 1; i < boardSize; ++i) {
        if (board[i][y].type == EMPTY) {
            boardSquares[i][y].setFillColor(sf::Color::Green);
        } else {
            if (board[i][y].color != selectedPiece->color) {
                boardSquares[i][y].setFillColor(sf::Color::Red);
            }
            break; // Block path
        }
    }

    // Horizontal moves to the left
    for (int i = x - 1; i >= 0; --i) {
        if (board[i][y].type == EMPTY) {
            boardSquares[i][y].setFillColor(sf::Color::Green);
        } else {
            if (board[i][y].color != selectedPiece->color) {
                boardSquares[i][y].setFillColor(sf::Color::Red);
            }
            break; // Block path
        }
    }

    // Vertical moves upwards
    for (int j = y - 1; j >= 0; --j) {
        if (board[x][j].type == EMPTY) {
            boardSquares[x][j].setFillColor(sf::Color::Green);
        } else {
            if (board[x][j].color != selectedPiece->color) {
                boardSquares[x][j].setFillColor(sf::Color::Red);
            }
            break; // Block path
        }
    }

    // Vertical moves downwards
    for (int j = y + 1; j < boardSize; ++j) {
        if (board[x][j].type == EMPTY) {
            boardSquares[x][j].setFillColor(sf::Color::Green);
        } else {
            if (board[x][j].color != selectedPiece->color) {
                boardSquares[x][j].setFillColor(sf::Color::Red);
            }
            break; // Block path
        }
    }
}

    // Highlight moves for King 
    if (selectedPiece->type == KING) {
        // Get the position of the opponent's King
        int opponentKingX = -1, opponentKingY = -1;
        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j].type == KING && board[i][j].color != selectedPiece->color) {
                    opponentKingX = i;
                    opponentKingY = j;
                    break;
                }
            }
            if (opponentKingX != -1) break; // Exit loop once opponent's King is found
        }

        // Check all 8 possible directions around the King
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int newX = x + dx;
                int newY = y + dy;

                // Ensure the move stays within the board limits
                if (newX >= 0 && newX < boardSize && newY >= 0 && newY < boardSize) {
                    // Temporarily move the King to the new position to simulate the move
                    Piece originalPiece = board[newX][newY];
                    board[newX][newY] = board[x][y];
                    board[x][y].type = EMPTY;

                    // Check if the King would be in check after this move
                    bool inCheck = false;
                    for (int i = 0; i < boardSize; ++i) {
                        for (int j = 0; j < boardSize; ++j) {
                            if (board[i][j].type != EMPTY && board[i][j].color != selectedPiece->color) {
                                if (board[i][j].isValidMove(i, j, newX, newY, board,enPassantTarget)) {
                                    inCheck = true;
                                    break;
                                }
                            }
                        }
                        if (inCheck) break;
                    }

                    // Revert the simulated move
                    board[x][y] = board[newX][newY];
                    board[newX][newY] = originalPiece;

                    // Check if the move is adjacent to the opponent's King
                    bool adjacentToOpponentKing = false;
                    if (opponentKingX != -1) { // If opponent King exists
                        if (abs(opponentKingX - newX) <= 1 && abs(opponentKingY - newY) <= 1) {
                            adjacentToOpponentKing = true;
                        }
                    }

                    // Highlight the move if it doesn't put the King in check and is not adjacent to the opponent's King
                    if (!inCheck && !adjacentToOpponentKing) {
                        // The King can move to an empty square
                        if (board[newX][newY].type == EMPTY) {
                            boardSquares[newX][newY].setFillColor(sf::Color::Green);
                        }
                        // The King can capture an opponent's piece (not the same color)
                        else if (board[newX][newY].color != selectedPiece->color) {
                            boardSquares[newX][newY].setFillColor(sf::Color::Red);
                        }
                    }
                }
            }
        }
    }
}

void ChessBoard::movePiece(Piece *piece, int x, int y){


    int startX = piece->sprite.getPosition().x / tileSize;
    int startY = piece->sprite.getPosition().y / tileSize;

    // Handle promotion

    if (piece->type == PAWN && ((piece->color == WHITE && y == 0) || (piece->color == BLACK && y == 7))) {
        piece->type = QUEEN; 
        std::string pieceName = (piece->color == WHITE ? "white_" : "black_") + pieceTypeName(piece->type);
        piece->setSprite(textures[pieceName]);
        piece->sprite.setPosition(x * tileSize, y * tileSize);  // Ensure correct sprite position
        board[x][y] = *piece;
    }

    if(board[x][y].type == EMPTY || board[x][y].color != piece->color) {
        piece->sprite.setPosition(x * tileSize, y * tileSize);
        board[x][y] = *piece;
    }

    if(piece->type == KING && abs(x - startX) == 2 && startY == y) {
        int rookStartX = (x > startX) ? 7 : 0;
        int rookEndX = (x > startX) ? x - 1 : x + 1;

        board[rookEndX][y] = board[rookStartX][y];
        board[rookEndX][y].sprite.setPosition(rookEndX * tileSize, y * tileSize);
        board[rookStartX][y] = Piece();
    } 


    if (piece->type == PAWN && x == enPassantTarget.first && y == enPassantTarget.second) {
        // Remove the captured pawn (opponent's pawn)
        int capturedPawnY = (piece->color == WHITE) ? y + 1 : y - 1;
        board[x][capturedPawnY] = Piece(); // Empty the captured pawn's square
    }

    if (piece->type == PAWN && abs(startY - y) == 2) {
        // Pawn moves two squares forward, set the enPassant target
        enPassantTarget = {x, (startY + y) / 2};  // Target square for en passant capture
    } else {
        // Reset the enPassant target if the pawn doesn't move two squares
        enPassantTarget = {-1, -1};
    }

    board[startX][startY] = Piece();
    // Reset all squares to their original colors
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            boardSquares[i][j].setFillColor((i + j) % 2 == 0 ? sf::Color(235, 235, 208) : sf::Color(119, 148, 85));
        }
    }
}

void ChessBoard::nextTurn() {

    currentTurn = (currentTurn == WHITE_TURN) ? BLACK_TURN : WHITE_TURN;
}