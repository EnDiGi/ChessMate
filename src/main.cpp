
#include "../include/game.h"
#include "../include/engine.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>
#include <cassert>

int main()
{

    Game game;
    ////game.loadFromFen("8/pR1n2kp/2qp1pp1/b7/6N1/1B2Q2P/P4PP1/5K2 w - - 0 1");

    Engine engine(Color::BLACK);

    while (true)
    {

        // * Player move

        std::cout << "fen: " << game.getFen(game.board) << std::endl;

        game.printBoard();

        game.turn = Color::WHITE;

        if(game.isCheckmate()) {
            std::cout << "Black wins!" << std::endl; 
            return 0;
        }

        std::cout << "White's turn" << std::endl;

        std::string from;
        std::cout << "from: ";
        std::cin >> from;

        if (getColor(game.pieceAt(from)) != game.turn)
        {
            std::cout << "Choose a piece of your color: " << (game.turn == Color::WHITE ? "White" : "Black") << std::endl;
            continue;
        }
        if (game.pieceAt(from) == Piece::NO_PIECE)
        {
            std::cout << "Choose a piece" << std::endl;
            continue;
        }

        int fromPos = algebraicNotationToPosition(from);

        std::vector<Move> legalMoves = filterLegal(game.board, getMoves(game.board, fromPos), fromPos);

        game.printBoard(legalMoves, fromPos);

        std::string to;
        std::cout << "from: " << from << std::endl;
        std::cout << "  to: ";
        std::cin >> to;

        if(to == "") continue;

        int toPos = algebraicNotationToPosition(to);

        if (std::any_of(legalMoves.begin(), legalMoves.end(), [toPos](const Move &m)
                        { return m.to == toPos; }))
        {
            Move move = {fromPos, toPos};

            game.move(move);
        }
        else
        {
            std::cout << "ILLEGAL MOVE" << std::endl;
            continue;
        }

        // * Engine move

        
        game.turn = Color::BLACK;
        
        
        if(game.isCheckmate()) 
        {
            game.printBoard();   
            std::cout << "White wins!" << std::endl; 
            return 0;
        }
        
        Move engineMove = engine.chooseMove(game.board);

        game.move(engineMove);
        
        game.turn = game.turn == Color::WHITE ? Color::BLACK : Color::WHITE;
        
        game.printBoard();

        std::cout << "Engine moved " << positionToAlgebraic(engineMove.from) << " to " << positionToAlgebraic(engineMove.to) << std::endl;
        std::cout << "fen: " << game.getFen(game.board) << std::endl;
    }

    return 0;
}