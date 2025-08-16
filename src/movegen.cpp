
#include "../include/movegen.h"
#include "../include/utils.h"
#include "../include/definitions.h"
#include <iostream>
#include <algorithm>
#include <cstdint>

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

            moves.push_back({bishopSquare, posToCheck, board[posToCheck]});

            // There's an enemy piece and NOT an en passant square
            if (cellToCheck != Piece::NO_PIECE && (cellToCheck != Piece::bEnPassant || cellToCheck == Piece::wEnPassant))
            {
                break;
            }
        }
    }

    return moves;
}

std::vector<Move> getPawnMoves(Piece board[120], int pawnSquare)
{

    bool isWhitePawn = isWhite(board[pawnSquare]);
    int direction = isWhitePawn ? 1 : -1;

    std::vector<Move> moves;

    int forwardMoveOffset = -10 * direction;

    int posToCheck = pawnSquare + forwardMoveOffset;
    Piece cellToCheck = board[posToCheck];

    if (cellToCheck == Piece::NO_PIECE)
    {
        Move pushMove = Move{pawnSquare, posToCheck, board[posToCheck]};
        if ((pushMove.to / 10 == 2 && isWhitePawn) || (pushMove.to / 10 == 9 && !isWhitePawn))
        {
            pushMove.promotion = true;
        }
        moves.push_back(pushMove);

        int doubleForwardMoveOffset = -20 * direction;

        int posToCheck = pawnSquare + doubleForwardMoveOffset;
        Piece cellToCheck = board[posToCheck];

        if (cellToCheck == Piece::NO_PIECE && ((pawnSquare / 10 == 8 && isWhitePawn) || (pawnSquare / 10 == 3 && !isWhitePawn)))
        {

            Move doublePushMove = Move{pawnSquare, posToCheck, board[posToCheck]};

            doublePushMove.isDoublePush = true;
            doublePushMove.enPassantCaptureSquare = pawnSquare + forwardMoveOffset;

            moves.push_back(doublePushMove);
        }
    }

    int captureOffsets[2] = {-11, -9};
    for (int offset : captureOffsets)
    {
        int posToCheck = pawnSquare + (offset * direction);
        Piece cellToCheck = board[posToCheck];

        if (!isSameColor(cellToCheck, board[pawnSquare]) && cellToCheck != Piece::NO_PIECE && cellToCheck != Piece::OFFBOARD)
        {
            Move capture = Move{pawnSquare, posToCheck, board[posToCheck]};
            if ((capture.to / 10 == 2 && isWhitePawn) || (capture.to / 10 == 9 && !isWhitePawn))
            {
                capture.promotion = true;
            }
            moves.push_back(capture);
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

            moves.push_back({rookSquare, posToCheck, board[posToCheck]});

            // There's an enemy piece and NOT an en passant square
            if (cellToCheck != Piece::NO_PIECE && (cellToCheck != Piece::bEnPassant || cellToCheck == Piece::wEnPassant))
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

        moves.push_back({kingSquare, posToCheck, board[posToCheck]});
    }

    // * Check castling
    std::vector<int> rooksPositions = findPieces(board, (isWhite(board[kingSquare]) ? Piece::wRook : Piece::bRook));
    if (rooksPositions.empty())
        return moves;
    for (int rookPos : rooksPositions)
    {
        bool kingSide = rookPos - kingSquare > 0;
        bool queenSide = !kingSide;

        bool canCastleOnThisSide = true;

        // Cycles trough the pieces in between the king and the rook
        for (int i = kingSquare + (kingSide ? 1 : -1); (kingSide ? i < rookPos : i > rookPos); (kingSide ? i++ : i--))
        {
            // Castling is not possible because there are pieces in the way
            if (board[i] != Piece::NO_PIECE)
            {
                canCastleOnThisSide = false;
            }
        }

        int kingNewSquare = kingSquare + (kingSide ? 2 : -2);
        int rookNewSquare = kingNewSquare + (kingSide ? -1 : 1);

        if (canCastleOnThisSide && board[kingNewSquare] == Piece::NO_PIECE)
        {
            Move castleMove = Move{kingSquare, kingNewSquare, board[kingNewSquare], true, kingSide, rookPos, rookNewSquare, board[rookNewSquare]};
            moves.push_back(castleMove);
        }
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

        moves.push_back({knightSquare, posToCheck, board[posToCheck]});
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

bool hasCastleRight(uint8_t castleRights, Color color, bool kingside)
{
    if (color == Color::WHITE)
    {
        if (kingside && !(castleRights & WHITE_KINGSIDE))
            return false;
        if (!kingside && !(castleRights & WHITE_QUEENSIDE))
            return false;
    }
    else if (color == Color::BLACK)
    {
        if (kingside && !(castleRights & BLACK_KINGSIDE))
            return false;
        if (!kingside && !(castleRights & BLACK_QUEENSIDE))
            return false;
    }
    return true;
}

std::vector<Move> filterLegal(Piece board[120], std::vector<Move> moves, int pieceSquare, uint8_t &castleRights)
{
    std::vector<Move> legalMoves;

    for (Move move : moves)
    {
        if (!move.isCastle)
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

            // Undoes the move
            board[pieceSquare] = pieceMoving;
            board[move.to] = pieceCaptured;
        }
        else
        {
            bool castleLegal = true;
            Color kingColor = getColor(board[move.from]);

            if (!hasCastleRight(castleRights, kingColor, move.isKingside))
            {
                continue;
            }

            for (int i = move.from; (move.isKingside ? i <= move.to : i >= move.to); (move.isKingside ? i++ : i--))
            {
                bool squareIsSafe = isSafe(board, i, getColor(board[move.from]));

                if (!squareIsSafe)
                {
                    castleLegal = false;
                }
            }
            if (castleLegal)
                legalMoves.push_back(move);
        }
    }

    return legalMoves;
}

std::vector<Move> getAllMoves(Piece board[120], Color color, uint8_t &castleRights)
{

    std::vector<Move> moves;

    for (int i = 0; i < 120; i++)
    {
        Piece piece = board[i];
        if (piece == Piece::NO_PIECE || piece == Piece::OFFBOARD || (getColor(piece) != color))
            continue;

        std::vector<Move> pieceMoves = filterLegal(board, getMoves(board, i), i, castleRights);

        moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    return moves;
}