
#include "../include/game.h"
#include "../include/engine.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include <iostream>
#include <algorithm>
#include <cmath>

std::string getEvalText(int eval)
{
    if(abs(eval) < 10) return "position is equal";
    else if(abs(eval) < 100) return "position is roughly equal";
    else if(eval < -150) return "black is better";
    else if(eval > 150) return "white is better";
    else if(eval > 100) return "white is slightly better";
    else if(eval < -100) return "black is slightly better";
    return "error evaluating the position";
}

int main()
{

    std::cout << "\033[0m" << std::endl;

    Game game;
    game.turn = Color::WHITE;
    std::cout << "Fen: " << game.getFen(game.board) << std::endl;
    std::cout << "The engine is evaluating the position..." << std::endl;

    Engine engine(Color::BLACK, &game, 3);
    double eval = engine.chooseMove(game.castleRights).second;

    while (true)
    {

        // Updates the turn
        if (game.turn == Color::WHITE)
        {
            game.moves++;
        }

        // * Player move
        if (game.turn != engine.color)
        {
            // Displays game information
            std::cout << "Fen: " << game.getFen(game.board) << std::endl;
            std::cout << "Evaluation: " << std::showpos << eval / 100 << std::noshowpos << std::endl;
            std::cout << getEvalText(eval) << std::endl;

            game.printBoard();

            if (game.isCheckmate())
            {
                std::cout << getColorName(opponentColor(game.turn)) << " wins!" << std::endl;
                return 0;
            }

            std::cout << std::endl << getColorName(game.turn) << "'s turn" << std::endl;
            int kingSquare = std::find(game.board, game.board + 120, game.turn == Color::WHITE ? Piece::wKing : Piece::bKing) - game.board;

            // Displays the check message in red
            if (kingSquare != 120)
            {
                if (!isSafe(game.board, kingSquare, getColor(game.pieceAt(kingSquare))))
                {
                    std::cout << "\033[91mCheck!" << std::endl;
                }
            }
            std::cout << "\033[0m";

            std::string from;
            std::cout << "from: ";
            std::cin >> from;


            // Checks the validity of the user input
            if (from.length() != 2)
                continue;

            // Tells the user to pick a piece of their color if they pick a piece that is not of their color
            if (getColor(game.pieceAt(from)) != game.turn)
            {
                std::cout << "Choose a piece of your color: " << getColorName(game.turn) << std::endl;
                continue;
            }

            // Tells the user to pick a square containing a piece
            else if (game.pieceAt(from) == Piece::NO_PIECE)
            {
                std::cout << "Choose a piece" << std::endl;
                continue;
            }

            int fromPos = algebraicNotationToPosition(from);

            std::vector<Move> legalMoves = filterLegal(&game, getMoves(game.board, fromPos), fromPos);

            // Prints the board and shows which legal moves are possible
            game.printBoard(legalMoves, fromPos);

            std::string to;
            std::cout << "from: " << from << std::endl;
            std::cout << "  to: ";
            std::cin >> to;

            // Checks the validity of the user input
            if (to.length() != 2)
                continue;

            int toPos = algebraicNotationToPosition(to);

            // Finds a move whose to coordinate matches the to input writeen by the user
            auto it = std::find_if(legalMoves.begin(), legalMoves.end(), [toPos](const Move &m)
                                   { return m.to == toPos; });

            if (it != legalMoves.end())
            {
                Move move = *it;
                game.move(move);
            }
            else
            {
                std::cout << "\033[91mILLEGAL MOVE\033[0m" << std::endl;
                continue;
            }

            game.switchTurn();
        }

        game.printBoard();

        // * Engine move

        std::cout << "Fen: " << game.getFen(game.board) << std::endl;

        if (game.isCheckmate())
        {
            std::cout << getColorName(game.turn) << " wins!" << std::endl;
            return 0;
        }

        std::cout << std::endl
                  << getColorName(game.turn) << "'s turn" << std::endl;
        int kingSquare = std::find(game.board, game.board + 120, game.turn == Color::WHITE ? Piece::wKing : Piece::bKing) - game.board;

        if (kingSquare != 120)
        {
            if (!isSafe(game.board, kingSquare, getColor(game.pieceAt(kingSquare))))
            {
                std::cout << "\033[91mCheck!" << std::endl;
            }
        }
        std::cout << "\033[0m";

        std::cout << "The engine is thinking..." << std::endl;

        std::pair<Move, int> engineOutput = engine.chooseMove(game.castleRights);
        Move engineMove = engineOutput.first;
        eval = engineOutput.second;
        game.move(engineMove);

        game.turn = game.turn == Color::WHITE ? Color::BLACK : Color::WHITE;

        // Displays information about the engine's move
        std::cout << "Engine moved " << pieceToName(game.board[engineMove.to]) << " from " << positionToAlgebraic(engineMove.from) << " to " << positionToAlgebraic(engineMove.to) << std::endl;
    }

    return 0;
}