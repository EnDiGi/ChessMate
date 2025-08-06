
#include <string>
#include <vector>
#include "definitions.h"

class Board
{

public:
    std::string startposfen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Piece board[120] = {
        Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, 
        Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::bRook,    Piece::bKnight,  Piece::bBishop,  Piece::bQueen,   Piece::bKing,    Piece::bBishop,  Piece::bKnight,  Piece::bRook,    Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::bPawn,    Piece::bPawn,    Piece::bPawn,    Piece::bPawn,    Piece::bPawn,    Piece::bPawn,    Piece::bPawn,    Piece::bPawn,    Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::NO_PIECE, Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::wPawn,    Piece::wPawn,    Piece::wPawn,    Piece::wPawn,    Piece::wPawn,    Piece::wPawn,    Piece::wPawn,    Piece::wPawn,    Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::wRook,    Piece::wKnight,  Piece::wBishop,  Piece::wQueen,   Piece::wKing,    Piece::wBishop,  Piece::wKnight,  Piece::wRook,    Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD,
        Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, Piece::OFFBOARD, 
    };

    void print(std::vector<int> legalMoves = {});

    void move(int from, int to);

    Piece pieceAt(int pos);

    Piece pieceAt(std::string algebraic);

};