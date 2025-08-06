
#include "../include/utils.h"
#include "../include/definitions.h"
#include <string>

bool isWhite(Piece piece)
{
    return piece <= Piece::bKing && piece > Piece::NO_PIECE;
}

bool isSameColor(Piece a, Piece b)
{
    if(a == Piece::OFFBOARD || b == Piece::OFFBOARD || a == Piece::NO_PIECE || b == Piece::NO_PIECE) return false;
    return isWhite(a) == isWhite(b);
}

Color getColor(Piece p)
{
    return isWhite(p) ? Color::WHITE : Color::BLACK;
}

int algebraicNotationToPosition(std::string notation)
{

    if (notation.length() < 2) return 0;

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
