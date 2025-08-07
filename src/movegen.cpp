
#include "../include/movegen.h"
#include "../include/utils.h"
#include <iostream>
#include <algorithm>

std::vector<Move> getBishopMoves(Piece board[120], int bishopSquare)
{

    std::vector<Move> moves = {};

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

            moves.push_back({bishopSquare, posToCheck});

            // There's an enemy piece
            if (cellToCheck != Piece::NO_PIECE)
            {
                break;
            }
        }
    }

    return moves;
}

std::vector<Move> getPawnMoves(Piece board[120], int pawnSquare)
{

    int direction = isWhite(board[pawnSquare]) ? 1 : -1;

    std::vector<Move> moves;

    int forwardMoveOffset = -10 * direction;

    int posToCheck = pawnSquare + forwardMoveOffset;
    Piece cellToCheck = board[posToCheck];

    if (cellToCheck == Piece::NO_PIECE)
    {
        moves.push_back({pawnSquare, posToCheck});

        int doubleForwardMoveOffset = -20 * direction;

        int posToCheck = pawnSquare + doubleForwardMoveOffset;
        Piece cellToCheck = board[posToCheck];

        if (cellToCheck == Piece::NO_PIECE)
        {
            moves.push_back({pawnSquare, posToCheck});
        }
    }

    return moves;
}

std::vector<Move> getRookMoves(Piece board[120], int rookSquare)
{
    std::vector<Move> moves = {};

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

            moves.push_back({rookSquare, posToCheck});

            // There's an enemy piece
            if (cellToCheck != Piece::NO_PIECE)
            {
                break;
            }
        }
    }

    return moves;
}

std::vector<Move> getQueenMoves(Piece board[120], int queenSquare)
{
    std::vector<Move> rookMoves = getRookMoves(board, queenSquare);
    std::vector<Move> bishopMoves = getBishopMoves(board, queenSquare);

    std::vector<Move> queenMoves = rookMoves;
    queenMoves.insert(queenMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return queenMoves;
}

std::vector<Move> getKingMoves(Piece board[120], int kingSquare)
{
    std::vector<Move> moves = {};

    std::vector<int> directions = {-10, 10, 11, -11, 9, -9, 1, -1};

    for (int dir : directions)
    {

        int posToCheck = kingSquare + dir;
        Piece cellToCheck = board[posToCheck];

        if (cellToCheck == Piece::OFFBOARD || isSameColor(cellToCheck, board[kingSquare]))
        {
            continue;
        }

        // TODO: Implement check check

        moves.push_back({kingSquare, posToCheck});
    }

    return moves;
}

std::vector<Move> getKnightMoves(Piece board[120], int knightSquare)
{

    std::vector<Move> moves = {};

    std::vector<int> directions = {+21, +19, +12, +8, -21, -19, -12, -8};

    for (int dir : directions)
    {

        int posToCheck = knightSquare + dir;
        Piece cellToCheck = board[posToCheck];

        if (cellToCheck == Piece::OFFBOARD || isSameColor(cellToCheck, board[knightSquare]))
        {
            continue;
        }

        moves.push_back({knightSquare, posToCheck});
    }

    return moves;
}

std::vector<Move> getMoves(Piece board[120], int pieceSquare)
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

bool isSafe(Piece board[120], int square, Color pieceColor)
{
    for (int i = 0; i < 120; i++)
    {
        Piece piece = board[i];
        if (getColor(piece) == pieceColor)
            continue; // Skips pieces of the same color
        std::vector<Move> pieceMoves = getMoves(board, i);
        // Only considers pawn diagonal moves
        if (piece == Piece::bPawn || piece == Piece::wPawn)
        {
            // Erases pawn forward move and double forward move from the vector for white and black pawns
            pieceMoves.erase(std::remove(pieceMoves.begin(), pieceMoves.end(), Move{i, i - 10}), pieceMoves.end());
            pieceMoves.erase(std::remove(pieceMoves.begin(), pieceMoves.end(), Move{i, i - 20}), pieceMoves.end());
            pieceMoves.erase(std::remove(pieceMoves.begin(), pieceMoves.end(), Move{i, i + 10}), pieceMoves.end());
            pieceMoves.erase(std::remove(pieceMoves.begin(), pieceMoves.end(), Move{i, i + 20}), pieceMoves.end());
        }
        if (std::any_of(pieceMoves.begin(), pieceMoves.end(), [square](const Move &m)
                        { return m.to == square; }))
        {
            return false;
        }
    }

    return true;
}

std::vector<Move> filterLegal(Piece board[120], std::vector<Move> moves, int pieceSquare)
{
    std::vector<Move> legalMoves;

    for (Move move : getMoves(board, pieceSquare))
    {
        Piece pieceCaptured = board[move.to];
        Piece pieceMoving = board[pieceSquare];

        // Makes the move
        board[move.to] = pieceMoving;
        board[pieceSquare] = Piece::NO_PIECE;

        int kingSquare = std::find(board, board + 120, isWhite(pieceMoving) ? Piece::wKing : Piece::bKing) - board;

        bool kingSafe = isSafe(board, kingSquare, getColor(pieceMoving));

        if (kingSafe)
        {
            legalMoves.push_back(move);
        }

        board[pieceSquare] = pieceMoving;
        board[move.to] = pieceCaptured;
    }

    return legalMoves;
}

std::vector<Move> getAllMoves(Piece board[120], Color color)
{

    std::vector<Move> moves;

    for (int i = 0; i < 120; i++)
    {
        Piece piece = board[i];
        if (piece == Piece::NO_PIECE || piece == Piece::OFFBOARD || (getColor(piece) != color))
            continue;

        std::vector<Move> pieceMoves = filterLegal(board, getMoves(board, i), i);

        moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    return moves;
}