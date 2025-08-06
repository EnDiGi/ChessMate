
#pragma once

enum class Piece {
    OFFBOARD = -1,
    NO_PIECE = 0,
    wKing, wQueen, wRook, wBishop, wKnight, wPawn,
    bKing, bQueen, bRook, bBishop, bKnight, bPawn, 
    PIECE_NB
};

enum class Color {
    WHITE = 0,
    BLACK = 1,
    COLOR_NB
};