
#include "../include/utils.h"
#include "../include/definitions.h"
#include <string>
#include <random>

bool isWhite(Piece piece)
{
    return piece < Piece::bKing && piece > Piece::NO_PIECE;
}

bool isSameColor(Piece a, Piece b)
{
    if (a == Piece::OFFBOARD || b == Piece::OFFBOARD || a == Piece::NO_PIECE || b == Piece::NO_PIECE)
        return false;
    return isWhite(a) == isWhite(b);
}

Color getColor(Piece p)
{
    return isWhite(p) ? Color::WHITE : Color::BLACK;
}

int algebraicNotationToPosition(std::string notation)
{

    if (notation.length() < 2)
        return 0;

    char colLetter = notation[0];
    char rowNumber = notation[1];

    int idx = (10 - (rowNumber - '0')) * 10 + (colLetter - 'a' + 1);

    return idx;
}

std::string positionToAlgebraic(int pos)
{
    char colLetter;
    char rowNumber;

    rowNumber = 10 - pos / 10 + '0';
    colLetter = pos % 10 - 1 + 'a';

    return std::string{colLetter, rowNumber};
}

char pieceToSymbol(Piece piece)
{
    switch (piece)
    {
    case Piece::bRook:
        return 'r';
        break;
    case Piece::bKnight:
        return 'n';
        break;
    case Piece::bBishop:
        return 'b';
        break;
    case Piece::bQueen:
        return 'q';
        break;
    case Piece::bKing:
        return 'k';
        break;
    case Piece::bPawn:
        return 'p';
        break;
    case Piece::wRook:
        return 'R';
        break;
    case Piece::wKnight:
        return 'N';
        break;
    case Piece::wBishop:
        return 'B';
        break;
    case Piece::wQueen:
        return 'Q';
        break;
    case Piece::wKing:
        return 'K';
        break;
    case Piece::wPawn:
        return 'P';
        break;
    default:
        return ' ';
        break;
    }
}

int randInt(int start, int end)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(start, end);

    return distr(gen);
}