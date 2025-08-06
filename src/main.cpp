
#include "../include/board.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>
#include <cassert>

int main()
{

    Board board;

    Color turn = Color::WHITE;

    while (true)
    {

        // * Player move

        board.print();
    
        std::cout << (turn == Color::WHITE ? "White" : "Black" )<< "'s turn" << std::endl;

        std::string from;
        std::cout << "from: ";
        std::cin >> from;

        if (board.pieceAt(from) == Piece::NO_PIECE || getColor(board.pieceAt(from)) != turn)
        {
            std::cout << "Choose a piece of your color" << std::endl;
            continue;
        }

        std::vector<int> legalMoves = getMoves(board.board, algebraicNotationToPosition(from));
        std::cout << "that piece has legal moves ";
        for (int move : legalMoves)
        {
            std::cout << positionToAlgebraic(move) << " ";
        }
        std::cout << "" << std::endl;

        board.print(legalMoves);

        std::string to;
        std::cout << "from: " << from << std::endl;
        std::cout << "  to: ";
        std::cin >> to;

        if (contains(legalMoves, algebraicNotationToPosition(to)))
        {
            board.move(algebraicNotationToPosition(from), algebraicNotationToPosition(to));
            turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
        }
    }

    return 0;
}