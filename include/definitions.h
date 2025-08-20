
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
    wEnPassant, // Used to check for en passant by putting before a double pawn move 
    bKing,
    bQueen,
    bRook,
    bBishop,
    bKnight,
    bPawn,
    bEnPassant,
    PIECE_NB
};

const int pieceValues[(int)(Piece::PIECE_NB)] = {
    0,   // NO_PIECE
    10000, // WK
    900, // WQ
    500, // WR
    330, // WB
    320, // WN
    100, // WP
    0, // wEnPassant
    10000, 900, 500, 330, 320, 100, 0 // black
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
    Piece captured;

    bool operator==(const Move &other)
    {
        return from == other.from && to == other.to;
    }

    bool isCastle = false;
    bool isKingside = false;
    int rookSquareFrom = 0;
    int rookSquareTo = 0;
    Piece rookCaptured = Piece::NO_PIECE;

    int castleBroken = 0;

    bool isDoublePush = false;
    int enPassantCaptureSquare = 0;

    bool promotion = false;
};