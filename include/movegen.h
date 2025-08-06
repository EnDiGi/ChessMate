
#include <vector>
#include "definitions.h"

std::vector<int> getBishopMoves(Piece board[120], int bishopSquare);

std::vector<int> getPawnMoves(Piece board[120], int pawnSquare);

std::vector<int> getRookMoves(Piece board[120], int rookSquare);

std::vector<int> getQueenMoves(Piece board[120], int queenSquare);

std::vector<int> getKingMoves(Piece board[120], int kingSquare);

std::vector<int> getKnightMoves(Piece board[120], int knightSquare);

std::vector<int> getMoves(Piece board[120], int pieceSquare);

bool isSafe(Piece board[120], int square);