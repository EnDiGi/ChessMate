
#include "../include/engine.h"
#include "../include/definitions.h"
#include "../include/movegen.h"
#include "../include/utils.h"
#include <utility>


Engine::Engine(Color color)
{
    this->color = color;
}

Move Engine::chooseMove(Piece board[120])
{
    std::vector<Move> allMoves = getAllMoves(board, this->color);

    return allMoves[randInt(0, allMoves.size() - 1)];
}