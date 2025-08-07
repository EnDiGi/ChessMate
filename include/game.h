
#include <string>
#include <vector>
#include "definitions.h"
#include <cstdint>

class Game
{

public:
    std::string startposfen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Color turn = Color::WHITE;
    uint8_t castleRights = WHITE_KINGSIDE | WHITE_QUEENSIDE | BLACK_KINGSIDE | BLACK_QUEENSIDE;

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

    void loadFromFen(std::string fen);

    void getFen(Piece board[120]);

    void printBoard(std::vector<int> legalMoves = {}, int pieceSquare = -1);

    void move(int from, int to);

    Piece pieceAt(int pos);

    Piece pieceAt(std::string algebraic);

};