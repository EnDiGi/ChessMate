
#pragma once

#include <string>
#include <vector>
#include "definitions.hpp"
#include <cstdint>

class Game
{

public:
    std::string startposfen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Color turn = Color::WHITE;
    uint8_t castleRights = WHITE_KINGSIDE | WHITE_QUEENSIDE | BLACK_KINGSIDE | BLACK_QUEENSIDE;
    int halfMoves = 0;
    int moves = 0;

    int wKingPos = 0;
    int bKingPos = 0;

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

    Game();

    int getEnPassantSquare();

    void switchTurn();

    void loadFromFen(std::string fen);

    std::string getFen(Piece board[120]);

    void printBoard(std::vector<Move> legalMoves = {}, int pieceSquare = -1);

    void officialMove(Move& move);

    void move(Move& move);

    void undoMove(Move& move);

    Piece pieceAt(int pos);

    Piece pieceAt(std::string algebraic);

    bool isCheckmate();

    bool isStalemate();

    bool inCheck(Color color);

};