
#include "../include/game.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>

void Game::loadFromFen(std::string fen)
{

    // Adds the first 21 OFFBOARDs
    for (int i = 0; i < 21; i++)
    {
        board[i] = Piece::OFFBOARD;
    }

    int boardIdx = 21;

    for (int i = 0; i < fen.length(); i++)
    {
        char ch = fen[i];

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
            case ' ':
                i++;
                // Cycles through castling rights
                for (int j = i; j < fen.length(); j++)
                {
                    if (fen[j] == ' ' || fen[j] == '-')
                    {
                        // Advances to the next char, the turn character
                        j++;
                        // Updates the turn
                        turn = fen[j] == 'w' ? Color::WHITE : Color::BLACK;
                        return;
                    }

                    switch (fen[j])
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
                }
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
    }
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

        if (piece == Piece::NO_PIECE)
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

    return fen;
}

void Game::printBoard(std::vector<Move> legalMoves, int pieceSquare)
{
    for (int i = 0; i < 120; i++)
    {
        // If i is a multiple of ten bigger than or equal to 20
        if (i >= 20 && i % 10 == 1)
        {
            std::cout << 8 - (i / 10 - 2) << " ";
        }

        Piece piece = board[i];

        bool canBeCaptured = std::any_of(legalMoves.begin(), legalMoves.end(), [i](const Move &m)
                                         { return m.to == i; });

        bool isSelected = i == pieceSquare;
        bool isMove = !legalMoves.empty() && canBeCaptured && piece == Piece::NO_PIECE;
        bool isCapture = !legalMoves.empty() && canBeCaptured && piece != Piece::NO_PIECE;

        if (piece != Piece::OFFBOARD)
        {
            std::cout << " ";
        }

        if (isSelected)
            std::cout << "\033[93m";
        if (isMove)
            std::cout << "\033[92m";
        if (isCapture)
            std::cout << "\033[91m";

        switch (piece)
        {
        case Piece::OFFBOARD:
            do
            {
                i++;
            } while (board[i] == Piece::OFFBOARD);
            i--;
            std::cout << std::endl;
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
            else
            {
                std::cout << " ";
            }
            break;
        }

        if (isSelected || isMove || isCapture)
            std::cout << "\033[0m";
    }

    std::cout << "\n   a b c d e f g h\n\n";
}

void Game::move(Move move)
{
    Piece piece = board[move.from];

    board[move.to] = piece;
    board[move.from] = Piece::NO_PIECE;
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
    if(getAllMoves(board, turn).size() == 0) return true;
    return false;
}