
#include "../include/engine.h"
#include "../include/definitions.h"
#include "../include/movegen.h"
#include "../include/utils.h"
#include <iostream>
#include <utility>
#include <chrono>
#include <algorithm>

typedef std::chrono::system_clock::time_point time_point_t;

Engine::Engine(Color color, Game *game, uint minDepth, uint maxDepth, double thinkingTime)
{
    this->color = color;
    this->game = game;
    this->evaluator = Evaluator();
    this->minDepth = minDepth;
    this->maxDepth = maxDepth;
    this->thinkingTime = thinkingTime;

}

std::pair<Move, int> Engine::chooseMove(uint8_t &castleRights)
{

    long long counter = 0;
    time_point_t start = std::chrono::system_clock::now();

    Move bestMoveSoFar;
    int eval = 0;

    int depthReached = 0;

    // Implement iterative deepening
    for(int d = 1; d <= maxDepth; d++)
    {
        std::cout << "searching at a depth of " << d << "..." << std::endl;
        Move currentBest;
        int currentEval = search(d, -1000000, 1000000, game->turn, currentBest, castleRights, counter, start);

        depthReached = d;

        if(timeUp(start) && d >= minDepth) break;

        bestMoveSoFar = currentBest;
        eval = currentEval;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto duration = elapsed.count();

    // Display info for how many positions were evaluated in how much time
    std::cout << "Analized " << counter << " positions in " << duration << "s reaching a depth of " << depthReached << " plies" << std::endl;

    return {bestMoveSoFar, eval * (this->color == Color::BLACK ? -1 : 1)};
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

bool Engine::timeUp(time_point_t start)
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto duration = elapsed.count();
    return duration >= thinkingTime;
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

int Engine::search(int depth, int alpha, int beta, Color color, Move &bestMove, uint8_t castleRights, long long &counter, time_point_t timerStart)
{

    if (depth == 0)
        return -quiescence(-1000000, 1000000, opponentColor(color), castleRights, counter, timerStart);


    if(timeUp(timerStart) && depth >= minDepth)
    {
        return alpha;
    }
    
    std::vector<Move> allMoves = getAllMoves(game, color);
    sortMoves(allMoves);
    
    if(allMoves.empty()) {
        // No move available: checkmate or stalemate
        if(game->inCheck(color)) return -100000 + depth;
        return 0;
    }
    
    counter++;
    
    Move localBest;

    for (Move move : allMoves)
    {
        game->move(move);

        uint8_t newCastleRights = castleRights;
        Move tempMove;
        int searchDepthExtension = 0;
        if (move.promotion || game->inCheck(opponentColor(color)))
            searchDepthExtension++;
        int eval = -search(depth - 1 + searchDepthExtension, -beta, -alpha, opponentColor(color), tempMove, newCastleRights, counter, timerStart);
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

int Engine::quiescence(int alpha, int beta, Color color, uint8_t castleRights, long long &counter, time_point_t timeStart)
{

    int eval = evaluator.eval(game->board) * (color == Color::WHITE ? 1 : -1);

    if(eval >= beta) return beta;
    alpha = std::max(alpha, eval);

    if(timeUp(timeStart)) return alpha;

    counter++;

    std::vector<Move> allCaptures = getAllMoves(game, color, true);
    sortMoves(allCaptures);

    for (Move move : allCaptures)
    {
        game->move(move);
        uint8_t newCastleRights = castleRights;
        eval = -quiescence(-beta, -alpha, opponentColor(color), newCastleRights, counter, timeStart);
        game->undoMove(move);

        if (eval >= beta)
            return beta;
        alpha = std::max(alpha, eval);
    }

    return alpha;
}