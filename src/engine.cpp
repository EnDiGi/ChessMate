
#include "../include/engine.h"
#include "../include/definitions.h"
#include "../include/movegen.h"
#include "../include/utils.h"
#include <iostream>
#include <utility>
#include <chrono>

Engine::Engine(Color color, Game *game, int depth)
{
    this->color = color;
    this->game = game;
    this->evaluator = Evaluator();
    this->depth = depth;
}

std::pair<Move, double> Engine::chooseMove(uint8_t &castleRights)
{
    long long counter = 0;
    auto start = std::chrono::high_resolution_clock::now();

    Move bestMove;
    int eval = search(depth, -1000000, 1000000, color, bestMove, castleRights, counter);

    std::chrono::duration<double, std::milli> elapsed = (std::chrono::high_resolution_clock::now() - start);
    auto duration = elapsed.count();
    std::cout << "Analized " << counter << " moves in " << duration / 1000 << "s with a depth of " << this->depth << " plies" << std::endl;

    return {bestMove, -eval};
}

int Engine::getMoveScore(const Move move)
{
    int score = 0;

    // Adds to the score the value of the captured piece
    if (move.captured != Piece::NO_PIECE)
        score += 10 * (getPieceValue(move.captured) - getPieceValue(game->board[move.to]));

    // Adds the value of the piece that the pawn promotes to
    if (move.promotion)
        score += getPieceValue(game->board[move.to]);

    return score;
}

void Engine::sortMoves(std::vector<Move> &moves)
{

    // Create pairs of the move score and the Move
    std::vector<std::pair<int, Move>> tmp;
    tmp.reserve(moves.size());
    for (Move move : moves)
    {
        tmp.emplace_back(getMoveScore(move), move);
    }

    // Sort the list based on the move score
    std::sort(tmp.begin(), tmp.end(),
              [](const auto &a, const auto &b)
              {
                  return a.first > b.first;
              });

    for (size_t i = 0; i < moves.size(); ++i)
    {
        moves[i] = tmp[i].second;
    }
}

double Engine::search(int depth, double alpha, double beta, Color color, Move &bestMove, uint8_t castleRights, long long &counter)
{

    if (depth == 0)
        return evaluator.eval(game->board) * (color == this->color ? -1 : 1);

    counter++;
    Move localBest;

    std::vector<Move> allMoves = getAllMoves(game, color);

    sortMoves(allMoves);

    for (Move move : allMoves)
    {
        game->move(move);

        uint8_t newCastleRights = castleRights;
        Move tempMove;
        int searchDepthExtension = 0;
        if (move.promotion)
            searchDepthExtension++;

        int eval = -search(depth - 1 + searchDepthExtension, -beta, -alpha, opponentColor(color), tempMove, newCastleRights, counter);
        game->undoMove(move);

        if (eval >= beta)
            return beta;

        if (eval > alpha)
        {
            alpha = eval;
            localBest = move;
        }
    }

    bestMove = localBest;

    return alpha;
}