
#include "../include/movegen.h"
#include "../include/utils.h"
#include<iostream>

std::vector<int> getBishopMoves(Piece board[120], int bishopSquare)
{

    std::vector<int> moves = {};

    std::vector<int> offsets = {-11, 11, -9, 9};

    for (int offset : offsets)
    {
        for (int i = 1; i < 8; i++)
        {
            int posToCheck = bishopSquare + offset * i;
            Piece cellToCheck = board[posToCheck];

            if (cellToCheck == Piece::OFFBOARD || isSameColor(cellToCheck, board[bishopSquare]))
            {
                break;
            }

            moves.push_back(posToCheck);

            // There's an enemy piece
            if (cellToCheck != Piece::NO_PIECE)
            {
                break;
            }
        }
    }

    return moves;
}

std::vector<int> getPawnMoves(Piece board[120], int pawnSquare)
{

    int direction = isWhite(board[pawnSquare]) ? 1 : -1;

    std::vector<int> moves;

    int forwardMoveOffset = -10 * direction;

    int posToCheck = pawnSquare + forwardMoveOffset;
    Piece cellToCheck = board[posToCheck];

    if (cellToCheck == Piece::NO_PIECE)
    {
        moves.push_back(posToCheck);

        int doubleForwardMoveOffset = -20 * direction;

        int posToCheck = pawnSquare + doubleForwardMoveOffset;
        Piece cellToCheck = board[posToCheck];
        
        if(cellToCheck == Piece::NO_PIECE)
        {
            moves.push_back(posToCheck);
        }
    }

    return moves;
}

std::vector<int> getRookMoves(Piece board[120], int rookSquare)
{
    std::vector<int> moves = {};

    std::vector<int> offsets = {-10, 10, 1, -1};

    for (int offset : offsets)
    {
        for (int i = 1; i < 8; i++)
        {
            int posToCheck = rookSquare + offset * i;
            Piece cellToCheck = board[posToCheck];

            if (cellToCheck == Piece::OFFBOARD || isSameColor(cellToCheck, board[rookSquare]))
            {
                break;
            }

            moves.push_back(posToCheck);

            // There's an enemy piece
            if (cellToCheck != Piece::NO_PIECE)
            {
                break;
            }
        }
    }

    return moves;
}

std::vector<int> getQueenMoves(Piece board[120], int queenSquare)
{
    std::vector<int> rookMoves = getRookMoves(board, queenSquare);
    std::vector<int> bishopMoves = getBishopMoves(board, queenSquare);

    std::vector<int> queenMoves = rookMoves;
    queenMoves.insert(queenMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return queenMoves;
}

std::vector<int> getKingMoves(Piece board[120], int kingSquare)
{
    std::vector<int> moves = {};

    std::vector<int> directions = {-10, 10, 11, -11, 9, -9, 1, -1};

    for (int dir : directions)
    {

        int posToCheck = kingSquare + dir;
        Piece cellToCheck = board[posToCheck];

        if (cellToCheck == Piece::OFFBOARD || isSameColor(cellToCheck, board[kingSquare]) || !isSafe(board, posToCheck))
        {
            break;
        }

        // TODO: Implement check check

        moves.push_back(posToCheck);

        // There's an enemy piece
        if (cellToCheck != Piece::NO_PIECE)
        {
            break;
        }
    }

    return moves;
}

std::vector<int> getKnightMoves(Piece board[120], int knightSquare)
{

    std::cout << "getknightmvoes" << std::endl;

    std::vector<int> moves = {};

    std::vector<int> directions = {+21, +19, +12, +8, -21, -19, -12, -8};

    for (int dir : directions)
    {

        int posToCheck = knightSquare + dir;
        Piece cellToCheck = board[posToCheck];

        if (cellToCheck == Piece::OFFBOARD || isSameColor(cellToCheck, board[knightSquare]))
        {
            continue;
        }

        moves.push_back(posToCheck);

        // There's an enemy piece
        if (cellToCheck != Piece::NO_PIECE)
        {
            break;
        }
    }

    return moves;
}

std::vector<int> getMoves(Piece board[120], int pieceSquare)
{
    switch (board[pieceSquare])
    {
    case Piece::bKing:
    case Piece::wKing:
        return getKingMoves(board, pieceSquare);
    case Piece::bQueen:
    case Piece::wQueen:
        return getQueenMoves(board, pieceSquare);
    case Piece::bRook:
    case Piece::wRook:
        return getRookMoves(board, pieceSquare);
    case Piece::bKnight:
    case Piece::wKnight:
        return getKnightMoves(board, pieceSquare);
    case Piece::bBishop:
    case Piece::wBishop:
        return getBishopMoves(board, pieceSquare);
    case Piece::bPawn:
    case Piece::wPawn:
        return getPawnMoves(board, pieceSquare);
    default:
        return {};
    }
}

bool isSafe(Piece board[120], int square)
{
    std::vector<int> unsafeSquares;

    for(int i = 0; i < 120; i++)
    {
        Piece piece = board[i];
        std::vector<int> pieceMoves = getMoves(board, i);
        unsafeSquares.insert(unsafeSquares.end(), pieceMoves.begin(), pieceMoves.end());
    }

    return contains(unsafeSquares, square);
}