
#pragma once

enum class Piece
{
    OFFBOARD = -1,
    NO_PIECE = 0,
    wKing,
    wQueen,
    wRook,
    wBishop,
    wKnight,
    wPawn,
    bKing,
    bQueen,
    bRook,
    bBishop,
    bKnight,
    bPawn,
    PIECE_NB
};

enum class Color
{
    WHITE = 0,
    BLACK = 1,
    COLOR_NB
};

enum castleRights
{
    WHITE_KINGSIDE = 1 << 0,
    WHITE_QUEENSIDE = 1 << 1,
    BLACK_KINGSIDE = 1 << 2,
    BLACK_QUEENSIDE = 1 << 3
};

struct Move
{
    int from;
    int to;

    bool operator==(const Move &other)
    {
        return from == other.from && to == other.to;
    }
};