
#include "../include/game.hpp"
#include "../include/engine.hpp"
#include "../include/utils.hpp"
#include "../include/movegen.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

std::string getEvalText(int eval)
{
    if (abs(eval) < 10)
        return "position is equal";
    else if (abs(eval) <= 100)
        return "position is roughly equal";
    else if (abs(eval) + 20 >= MATE_SCORE)
        return std::string("FORCED MATE for ") + (eval > 0 ? "white" : "black");
    else if (eval < -150)
        return "black is better";
    else if (eval > 150)
        return "white is better";
    else if (eval >= 100)
        return "white is slightly better";
    else if (eval <= -100)
        return "black is slightly better";
    return "error evaluating the position";
}

int main(int argc, char *argv[])
{

    Game game;
    game.turn = Color::WHITE;

    Color engineColor = Color::BLACK;
    uint engineMaxDepth = 4;
    uint engineMinDepth = 2;
    uint engineThinkingTime = 10;

    std::string fen = game.startposfen;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--color" && i + 1 < argc)
        {

            char colorStr = *argv[i + 1];
            if (colorStr == 'w')
            {
                engineColor = Color::WHITE;
            }
        }
        else if (arg == "--max_depth" && i + 1 < argc)
        {
            uint depthInput = std::atoi(argv[++i]);
            if (depthInput > 2)
            {
                engineMaxDepth = depthInput;
            }
        }
        else if (arg == "--min_depth" && i + 1 < argc)
        {
            uint depthInput = std::atoi(argv[++i]);
            if (depthInput > 2)
            {
                engineMinDepth = depthInput;
            }
        }
        else if (arg == "--think_time" && i + 1 < argc)
        {
            uint timeInput = std::atoi(argv[++i]);
            if (timeInput > 0)
            {
                engineThinkingTime = timeInput;
            }
        }
        else if (arg == "--fen" && i + 1 < argc)
        {
            fen = argv[i+1];
            i++;
        }
    }

    game.loadFromFen(fen);

    std::cout << "\033[0m" << std::endl;

    Engine engine(engineColor, &game, engineMinDepth, engineMaxDepth, engineThinkingTime);
    std::cout << "Engine initialized with color: " << getColorName(engine.color) << std::endl
              << "                minimum depth: " << engine.minDepth << std::endl
              << "                maximum depth: " << engine.maxDepth << std::endl
              << "                thinking time: " << engine.thinkingTime << "s" << std::endl;
    std::cout << "The engine is evaluating the position..." << std::endl;

    double eval = engine.chooseMove(game.castleRights).second;

    std::cout << "Fen: " << game.getFen(game.board) << std::endl;

    while (true)
    {

        // Updates the move count
        if (game.turn == Color::WHITE)
        {
            game.moves++;
            // Displays game information
            std::cout << "Fen: " << game.getFen(game.board) << std::endl;
            std::cout << "Evaluation: " << std::showpos << eval / 100 << std::noshowpos << std::endl;
            std::cout << getEvalText(eval) << std::endl;
        }

        // * Player move
        if (game.turn != engine.color)
        {

            game.printBoard();

            if (game.isCheckmate())
            {
                std::cout << getColorName(opponentColor(game.turn)) << " wins!" << std::endl;
                return 0;
            }
            else if (game.isStalemate())
            {
                std::cout << "Stalemate!" << std::endl;
                return 0;
            }

            std::cout << std::endl
                      << getColorName(game.turn) << "'s turn" << std::endl;
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

            // Finds a move whose to coordinate matches the to input written by the user
            auto it = std::find_if(legalMoves.begin(), legalMoves.end(), [toPos](const Move &m)
                                   { return m.to == toPos; });

            if (it != legalMoves.end())
            {
                Move move = *it;
                game.officialMove(move);
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

        if (game.isCheckmate())
        {
            std::cout << getColorName(opponentColor(game.turn)) << " wins!" << std::endl;
            return 0;
        }
        else if (game.isStalemate())
        {
            std::cout << "Stalemate!" << std::endl;
            return 0;
        }

        std::cout << "Fen: " << game.getFen(game.board) << std::endl;

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
        game.officialMove(engineMove);

        game.turn = game.turn == Color::WHITE ? Color::BLACK : Color::WHITE;

        // Displays information about the engine's move
        std::cout << "Engine moved " << pieceToName(game.board[engineMove.to]) << " from " << positionToAlgebraic(engineMove.from) << " to " << positionToAlgebraic(engineMove.to) << std::endl;
    }

    return 0;
}