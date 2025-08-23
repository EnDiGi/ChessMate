
#include "../include/evaluator.hpp"
#include "../include/definitions.hpp"
#include "../include/movegen.hpp"
#include "../include/utils.hpp"
#include <array>
#include <iostream>

std::array<int, 80> Evaluator::getPieceTable(Piece piece)
{
    switch (piece)
    {
    case Piece::wPawn:
        return wPawnTable;
        break;
    case Piece::bPawn:
        return bPawnTable;
        break;
    case Piece::wKnight:
        return wKnightTable;
        break;
    case Piece::bKnight:
        return bKnightTable;
        break;
    case Piece::wBishop:
        return wBishopTable;
        break;
    case Piece::bBishop:
        return bBishopTable;
        break;
    case Piece::wRook:
        return wRookTable;
        break;
    case Piece::bRook:
        return bRookTable;
        break;
    case Piece::wQueen:
        return wQueenTable;
        break;
    case Piece::bQueen:
        return bQueenTable;
        break;
    case Piece::wKing:
        return wKingTable;
        break;
    case Piece::bKing:
        return bKingTable;
        break;
    default:
        return {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        break;
    }
}

// returns the board evaluation from white's perspective
int Evaluator::eval(Piece board[120])
{
    // Evalutation from white perspective
    int evaluation = 0;

    for (int i = 21; i < 99; i++)
    {
        Piece piece = board[i];

        if (piece == Piece::OFFBOARD || piece == Piece::NO_PIECE)
        {
            continue;
        }

        int pieceEval = pieceValues[(int)piece];

        // Adds to the evaluation a value based on the piece's position
        pieceEval += this->getPieceTable(piece)[i - 20];

        int mobility = getMoves(board, i).size();
        switch (piece)
        {
        case Piece::wBishop:
        case Piece::bBishop:
            pieceEval += mobility * bishopMobilityBonus;
            break;
        case Piece::wKnight:
        case Piece::bKnight:
            pieceEval += mobility * knightMobilityBonus;
            break;
        case Piece::wRook:
        case Piece::bRook:
            pieceEval += mobility * rookMobilityBonus;
            break;
        case Piece::wQueen:
        case Piece::bQueen:
            pieceEval += mobility * queenMobilityBonus;
            break;
        default:
            break;
        }

        pieceEval *= (isWhite(piece) ? 1 : -1);
        evaluation += pieceEval;
    }

    return evaluation;
}