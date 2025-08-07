
#include "../include/game.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>
#include <cassert>

int main()
{

    Game game;
    game.loadFromFen("8/pR1n2kp/2qp1pp1/b7/6N1/1B2Q2P/P4PP1/5K2 w - - 0 1");

    Color turn = Color::WHITE;

    while (true)
    {

        // * Player move

        game.printBoard();

        std::cout << (turn == Color::WHITE ? "White" : "Black") << "'s turn" << std::endl;

        std::string from;
        std::cout << "from: ";
        std::cin >> from;

        if (getColor(game.pieceAt(from)) != turn)
        {
            std::cout << "Choose a piece of your color: " << (turn == Color::WHITE ? "White" : "Black") << std::endl;
            continue;
        }
        if (game.pieceAt(from) == Piece::NO_PIECE)
        {
            std::cout << "Choose a piece" << std::endl;
            continue;
        }

        int fromPos = algebraicNotationToPosition(from);

        std::vector<int> pseudoLegalMoves = getMoves(game.board, fromPos);
        std::cout << "that piece has pseudolegal moves ";
        for (int move : pseudoLegalMoves)
        {
            std::cout << positionToAlgebraic(move) << " ";
        }
        std::cout << "" << std::endl;
        std::vector<int> legalMoves = filterLegal(game.board, getMoves(game.board, fromPos), fromPos);
        std::cout << "that piece has legal moves ";
        for (int move : legalMoves)
        {
            std::cout << positionToAlgebraic(move) << " ";
        }
        std::cout << "" << std::endl;

        game.printBoard(legalMoves, fromPos);

        std::string to;
        std::cout << "from: " << from << std::endl;
        std::cout << "  to: ";
        std::cin >> to;

        if (contains(legalMoves, algebraicNotationToPosition(to)))
        {
            game.move(algebraicNotationToPosition(from), algebraicNotationToPosition(to));
            turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
        }
        else
        {
            std::cout << "ILLEGAL MOVE" << std::endl;
        }
    }

    return 0;
}