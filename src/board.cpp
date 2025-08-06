
#include "../include/board.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>

void Board::print(std::vector<int> legalMoves)
{
    for (int i = 0; i < 120; i++)
    {
        // If i is a multiple of ten bigger than or equal to 20
        if(i >= 20 && i % 10 == 1)
        {
            std::cout << 8 - (i / 10 - 2) << " ";
        }

        Piece piece = board[i];

        if(piece != Piece::OFFBOARD){
            std::cout << " ";
        }

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
            if(!legalMoves.empty() && contains(legalMoves, i))
            {
                std::cout << "*";
            } 
            else 
            {
                std::cout << " ";
            }
            break;
        }
    }

    std::cout << "\n   a b c d e f g h\n\n";
}

void Board::move(int from, int to)
{
    Piece piece = board[from];

    board[to] = piece;
    board[from] = Piece::NO_PIECE;
}

Piece Board::pieceAt(int pos)
{
    return board[pos];
}

Piece Board::pieceAt(std::string algebraic)
{
    return board[algebraicNotationToPosition(algebraic)];
}