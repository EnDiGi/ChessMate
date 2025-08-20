
#include "../include/game.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>
#include <algorithm>
#include <string>

Game::Game()
{
    loadFromFen(this->startposfen);
}

void Game::switchTurn()
{
    turn = opponentColor(turn);
}

void Game::loadFromFen(std::string fen)
{

    castleRights = 0;
    halfMoves = 0;
    moves = 0;

    int i = 0;

    // Adds the first 21 OFFBOARDs
    for (; i < 21; i++)
    {
        board[i] = Piece::OFFBOARD;
    }

    int boardIdx = 21;

    for (int j = 0; j < fen.length(); j++)
    {
        char ch = fen[j];

        if (std::isdigit(ch))
        {
            int spaces = ch - '0';
            for (int s = 0; s < spaces; s++)
            {
                board[boardIdx++] = Piece::NO_PIECE;
            }

            continue;
        }
        else if (ch == '/')
        {
            board[boardIdx++] = Piece::OFFBOARD;
            board[boardIdx++] = Piece::OFFBOARD;
        }
        else
        {
            switch (ch)
            {
            case 'r':
                board[boardIdx] = Piece::bRook;
                break;
            case 'n':
                board[boardIdx] = Piece::bKnight;
                break;
            case 'b':
                board[boardIdx] = Piece::bBishop;
                break;
            case 'q':
                board[boardIdx] = Piece::bQueen;
                break;
            case 'k':
                bKingPos = boardIdx;
                board[boardIdx] = Piece::bKing;
                break;
            case 'p':
                board[boardIdx] = Piece::bPawn;
                break;
            case 'R':
                board[boardIdx] = Piece::wRook;
                break;
            case 'N':
                board[boardIdx] = Piece::wKnight;
                break;
            case 'B':
                board[boardIdx] = Piece::wBishop;
                break;
            case 'Q':
                board[boardIdx] = Piece::wQueen;
                break;
            case 'K':
                wKingPos = boardIdx;
                board[boardIdx] = Piece::wKing;
                break;
            case 'P':
                board[boardIdx] = Piece::wPawn;
                break;
            default:
                break;
            }
            boardIdx++;
        }
        if (fen[j] == ' ')
        {
            i = j;
            break;
        }
    }
    i++;
    this->turn = (fen[i] == 'w' ? Color::WHITE : Color::BLACK);
    i += 2;
    // Cycles through castling rights
    for (; i < fen.length(); i++)
    {
        switch (fen[i])
        {
        case 'K':
            castleRights |= WHITE_KINGSIDE;
            break;
        case 'Q':
            castleRights |= WHITE_QUEENSIDE;
            break;
        case 'k':
            castleRights |= BLACK_KINGSIDE;
            break;
        case 'q':
            castleRights |= BLACK_QUEENSIDE;
            break;
        default:
            break;
        }
        if (fen[i] == ' ' || fen[i] == '-')
        {
            // Updates the en passant captures
            if (fen[i] != '-')
            {
                std::string notation = std::string{fen[i]} + fen[i++];
                int position = algebraicNotationToPosition(notation);

                int pawnSquare = board[position + 10] == Piece::NO_PIECE ? (position - 10) : (position + 10);
                board[position] = getColor(board[pawnSquare]) == Color::WHITE ? Piece::wEnPassant : Piece::bEnPassant;

                return;
            }
        }
    }
}

int Game::getEnPassantSquare()
{

    auto it = std::find_if(board, board + 120, [](Piece val)
                           { return val == Piece::wEnPassant || val == Piece::bEnPassant; });
    if (it != board + 120)
    {
        return it - board;
    }
    return 0;
}

std::string Game::getFen(Piece board[120])
{

    std::string fen = "";

    int currentSpaces = 0;
    for (int i = 21; i < 99; i++)
    {
        Piece piece = board[i];
        if (piece == Piece::OFFBOARD)
        {
            if (board[i + 1] == Piece::OFFBOARD)
            {
                if (currentSpaces != 0)
                {
                    fen += (char)currentSpaces + '0';
                    currentSpaces = 0;
                }
                fen += "/";
            }
            continue;
        }

        if (piece == Piece::NO_PIECE || piece == Piece::wEnPassant || piece == Piece::bEnPassant)
        {
            currentSpaces += 1;
        }
        else
        {
            if (currentSpaces != 0)
            {
                fen += (char)currentSpaces + '0';
                currentSpaces = 0;
            }
            fen += pieceToSymbol(piece);
        }
    }

    if (currentSpaces != 0)
    {
        fen += (char)currentSpaces + '0';
        currentSpaces = 0;
    }

    fen += std::string(" ") + (turn == Color::WHITE ? "w" : "b");

    fen += std::string(" ") + castleRightsToString(castleRights);

    int enPassantSquare = getEnPassantSquare();
    if (enPassantSquare == 0)
    {
        fen += std::string(" -");
    }
    else
    {
        fen += std::string(" ") + positionToAlgebraic(enPassantSquare);
    }

    fen += " " + std::to_string(halfMoves) + " " + std::to_string(moves);

    return fen;
}

void Game::printBoard(std::vector<Move> legalMoves, int pieceSquare)
{

    bool isSquareBlack = true;

    for (int i = 0; i < 99; i++)
    {
        // If i is a multiple of ten bigger than or equal to 20
        if (i >= 20 && i % 10 == 1)
        {
            std::cout << std::endl
                      << "\033[0;90m" << 8 - (i / 10 - 2) << " \033[0;0m";
            isSquareBlack = !isSquareBlack;
        }

        Piece piece = board[i];

        bool canMoveToThatSquare = std::any_of(legalMoves.begin(), legalMoves.end(), [i](const Move &m)
                                         { return m.to == i; });

        bool movingPieceIsPawn = (board[pieceSquare] == Piece::wPawn || board[pieceSquare] == Piece::bPawn);
        bool pieceIsEnPassant = piece == Piece::bEnPassant || piece == Piece::wEnPassant;

        bool isCapture = !legalMoves.empty() && 
                         canMoveToThatSquare &&
                         (piece != Piece::NO_PIECE && !pieceIsEnPassant) ||
                         (pieceIsEnPassant && movingPieceIsPawn);

        bool isEnPassant = isCapture && canMoveToThatSquare &&
                           pieceIsEnPassant &&
                           movingPieceIsPawn;

        bool isMove = !legalMoves.empty() && canMoveToThatSquare &&
                      (piece == Piece::NO_PIECE || pieceIsEnPassant) && !isEnPassant;

        std::string bgColor = isSquareBlack ? "104" : "46";
        std::string fgColor = getColor(piece) == Color::WHITE ? "97" : "30";

        if (isMove)
            fgColor = "34";
        if (isCapture)
            fgColor = "91";

        std::cout << "\033[" << bgColor << ";" << fgColor << "m";

        if (piece != Piece::OFFBOARD)
        {
            std::cout << " ";
        }
        else
        {
            std::cout << "\033[0m";
        }

        switch (piece)
        {
        case Piece::OFFBOARD:
            while (board[i] == Piece::OFFBOARD)
                i++;
            i--;
            continue;
        case Piece::wRook:
            std::cout << "R";
            break;
        case Piece::wKnight:
            std::cout << "N";
            break;
        case Piece::wBishop:
            std::cout << "B";
            break;
        case Piece::wQueen:
            std::cout << "Q";
            break;
        case Piece::wKing:
            std::cout << "K";
            break;
        case Piece::wPawn:
            std::cout << "P";
            break;
        case Piece::bRook:
            std::cout << "r";
            break;
        case Piece::bKnight:
            std::cout << "n";
            break;
        case Piece::bBishop:
            std::cout << "b";
            break;
        case Piece::bQueen:
            std::cout << "q";
            break;
        case Piece::bKing:
            std::cout << "k";
            break;
        case Piece::bPawn:
            std::cout << "p";
            break;
        default:
            if (isMove)
            {
                std::cout << "*";
            }
            else if (isEnPassant)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << " ";
            }
            break;
        }

        std::cout << "\033[0m";

        isSquareBlack = !isSquareBlack;
    }

    std::cout << "\n\n\033[90m   a b c d e f g h\033[0m\n\n";
}

void Game::officialMove(Move &move)
{
    this->move(move);
    
    // Eliminates any opponent enpassant pieces
    replace(board, 120, isWhite(board[move.to]) ? Piece::bEnPassant : Piece::wEnPassant, Piece::NO_PIECE);
}

void Game::move(Move &move)
{

    // A move object without any flags like castling, promotion, etc.
    Move rawMove = Move{move.from, move.to, move.captured};

    // Check if the move broke any casstle right
    {
        if (pieceAt(move.from) == Piece::wRook)
        {
            if (positionToAlgebraic(move.from) == "a1" && (castleRights & WHITE_QUEENSIDE))
            {
                castleRights &= ~WHITE_QUEENSIDE;
                move.castleBroken |= WHITE_QUEENSIDE;
            }
            if (positionToAlgebraic(move.from) == "h1" && (castleRights & WHITE_KINGSIDE))
            {
                castleRights &= ~WHITE_KINGSIDE;
                move.castleBroken |= WHITE_KINGSIDE;
            }
        }
        else if (pieceAt(move.from) == Piece::bRook)
        {
            if (positionToAlgebraic(move.from) == "a8" && (castleRights & BLACK_QUEENSIDE))
            {
                castleRights &= ~BLACK_QUEENSIDE;
                move.castleBroken |= BLACK_QUEENSIDE;
            }
            if (positionToAlgebraic(move.from) == "h8" && (castleRights & BLACK_KINGSIDE))
            {
                castleRights &= ~BLACK_KINGSIDE;
                move.castleBroken |= BLACK_KINGSIDE;
            }
            else if (pieceAt(move.from) == Piece::wKing && (castleRights & (WHITE_QUEENSIDE | WHITE_KINGSIDE)))
            {
                castleRights &= ~(WHITE_KINGSIDE | WHITE_QUEENSIDE);
                move.castleBroken |= (WHITE_KINGSIDE | WHITE_QUEENSIDE);
            }
            else if (pieceAt(move.from) == Piece::bKing && (castleRights & (BLACK_QUEENSIDE | BLACK_KINGSIDE)))
            {
                castleRights &= ~(BLACK_KINGSIDE | BLACK_QUEENSIDE);
                move.castleBroken |= (BLACK_KINGSIDE | BLACK_QUEENSIDE);
            }
        }
    }

    if (move.isDoublePush)
    {
        this->move(rawMove);
        board[move.enPassantCaptureSquare] = isWhite(board[move.to]) ? Piece::wEnPassant : Piece::bEnPassant;
    }
    else if (!move.isCastle)
    {
        Piece piece = board[move.from];
        board[move.to] = piece;
        board[move.from] = Piece::NO_PIECE;

        bool isEnPassant = move.captured == Piece::bEnPassant || move.captured == Piece::wEnPassant;
        if (isEnPassant)
        {
            if (move.captured == Piece::wEnPassant)
            {
                board[move.to - 10] = Piece::NO_PIECE;
            }
            else if (move.captured == Piece::bEnPassant)
            {
                board[move.to + 10] = Piece::NO_PIECE;
            }
        }
        if (move.promotion)
        {
            if (isWhite(board[move.to]))
                board[move.to] = Piece::wQueen;
            else
                board[move.to] = Piece::bQueen;
        }

        // Eliminates any opponent enpassant pieces because you can only en passant on the move after the double push
        replace(board, 120, isWhite(board[move.to]) ? Piece::bEnPassant : Piece::wEnPassant, Piece::NO_PIECE);
    }
    else
    {
        this->move(rawMove);
        Move rookMove = Move{move.rookSquareFrom, move.rookSquareTo, move.rookCaptured};
        this->move(rookMove);
    }

    // Updates variables
    if(board[move.to] == Piece::wKing) wKingPos = move.to;
    if(board[move.to] == Piece::bKing) bKingPos = move.to;
    if(move.captured == Piece::wKing) wKingPos = 0;
    if(move.captured == Piece::bKing) bKingPos = 0;
}

void Game::undoMove(Move &move)
{

    // A move object without any falgs like castling, promotion, etc.
    Move rawMove = Move{move.from, move.to, move.captured};

    castleRights |= move.castleBroken;

    if (move.isDoublePush)
    {
        this->undoMove(rawMove);
        replace(board, 120, isWhite(board[move.from]) ? Piece::wEnPassant : Piece::bEnPassant, Piece::NO_PIECE);
    }
    else if (!move.isCastle)
    {
        board[move.from] = board[move.to];
        board[move.to] = move.captured;

        bool isEnPassant = move.captured == Piece::bEnPassant || move.captured == Piece::wEnPassant;
        if (isEnPassant)
        {
            if (move.captured == Piece::wEnPassant)
            {
                board[move.to - 10] = Piece::wPawn;
            }
            else if (move.captured == Piece::bEnPassant)
            {
                board[move.to + 10] = Piece::bPawn;
            }
        }
        if (move.promotion)
        {
            if (isWhite(board[move.from]))
                board[move.from] = Piece::wPawn;
            else
                board[move.from] = Piece::bPawn;
        }
    }
    else
    {
        this->undoMove(rawMove);
        Move rookMove = Move{move.rookSquareFrom, move.rookSquareTo, move.rookCaptured};
        this->undoMove(rookMove);
        switch (getColor(board[move.from]))
        {
        case Color::WHITE:
            if (move.isKingside)
                castleRights |= WHITE_KINGSIDE;
            else
                castleRights |= WHITE_QUEENSIDE;
            break;
        case Color::BLACK:
            if (move.isKingside)
                castleRights |= BLACK_KINGSIDE;
            else
                castleRights |= BLACK_QUEENSIDE;
            break;
        default:
            break;
        }
    }

    // Updates variables
    if(board[move.from] == Piece::wKing) wKingPos = move.from;
    if(board[move.from] == Piece::bKing) bKingPos = move.from;
    if(move.captured == Piece::wKing) wKingPos = move.to;
    if(move.captured == Piece::bKing) bKingPos = move.to;
}

Piece Game::pieceAt(int pos)
{
    return board[pos];
}

Piece Game::pieceAt(std::string algebraic)
{
    return board[algebraicNotationToPosition(algebraic)];
}

bool Game::isCheckmate()
{
    return getAllMoves(this, turn).size() == 0;
}

bool Game::inCheck(Color color)
{
    int kingSquare = color == Color::WHITE ? wKingPos : bKingPos;
    return !isSafe(board, kingSquare, color);
}