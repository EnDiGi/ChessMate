
#include "../include/utils.h"
#include "../include/definitions.h"
#include <string>
#include <random>
#include <cstdint>

bool isWhite(Piece piece)
{
    return piece < Piece::bKing && piece > Piece::NO_PIECE;
}

std::string getColorName(Color color)
{
    return color == Color::WHITE ? "White" : "Black";
}

bool isSameColor(Piece a, Piece b)
{
    if (a == Piece::OFFBOARD || b == Piece::OFFBOARD || a == Piece::NO_PIECE || b == Piece::NO_PIECE)
        return false;
    return isWhite(a) == isWhite(b);
}

Color getColor(Piece p)
{
    if(((int)p < (int)Piece::wKing) || (int)p > (int)Piece::bEnPassant) return Color::COLOR_NB;
    return isWhite(p) ? Color::WHITE : Color::BLACK;
}

int algebraicNotationToPosition(std::string notation)
{

    if (notation.length() != 2)
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

int getPieceValue(Piece piece)
{
    return pieceValues[(int) piece];
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

std::string pieceToName(Piece piece)
{
    
    switch (piece)
    {
    case Piece::wRook:
    case Piece::bRook:
        return "rook";
        break;
    case Piece::wKnight:
    case Piece::bKnight:
        return "knight";
        break;
    case Piece::wBishop:
    case Piece::bBishop:
        return "bishop";
        break;
    case Piece::wQueen:
    case Piece::bQueen:
        return "queen";
        break;
    case Piece::wKing:
    case Piece::bKing:
        return "king";
        break;
    case Piece::wPawn:
    case Piece::bPawn:
        return "pawn";
        break;
    default:
        return " ";
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

std::vector<int> findPieces(Piece board[120], Piece value) {
    std::vector<int> indexes;
    for(int i = 0; i < 120; i++)
    {
        if(board[i] == value){
            indexes.push_back(i);
        }
    }
    return indexes;
}

std::string castleRightsToString(uint8_t flags) {
    std::string result;
    if (flags & WHITE_KINGSIDE)  result += "K";  // White King-side
    if (flags & WHITE_QUEENSIDE) result += "Q";  // White Queen-side
    if (flags & BLACK_KINGSIDE)  result += "k";  // Black King-side
    if (flags & BLACK_QUEENSIDE) result += "q";  // Black Queen-side
    if (result.empty()) result = "-";   // Nessun diritto
    return result;
}

Color opponentColor(Color color)
{
    return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}


std::string debugMove(const Move m)
{
    std::string output;
    output += "move from " + positionToAlgebraic(m.from) + " (" + std::to_string(m.from) + ") to " + positionToAlgebraic(m.to) + " (" + std::to_string(m.to)+ ")";
    if(m.captured != Piece::NO_PIECE) output += " capturing a " +std::string(1, pieceToSymbol(m.captured));
    if(m.isCastle) output += std::string(" castling ") + (m.isKingside ? "kingside" : "queenside") + ". Rook: " + debugMove(Move{m.rookSquareFrom, m.rookSquareTo});
    return output;
}