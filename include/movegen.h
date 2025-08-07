
#include <vector>
#include "definitions.h"

std::vector<Move> getBishopMoves(Piece board[120], int bishopSquare);

std::vector<Move> getPawnMoves(Piece board[120], int pawnSquare);

std::vector<Move> getRookMoves(Piece board[120], int rookSquare);

std::vector<Move> getQueenMoves(Piece board[120], int queenSquare);

std::vector<Move> getKingMoves(Piece board[120], int kingSquare);

std::vector<Move> getKnightMoves(Piece board[120], int knightSquare);

std::vector<Move> getMoves(Piece board[120], int pieceSquare);

bool isSafe(Piece board[120], int square, Color pieceColor);

std::vector<Move> filterLegal(Piece board[120], std::vector<Move> moves, int pieceSquare);

std::vector<Move> getAllMoves(Piece board[120], Color color);