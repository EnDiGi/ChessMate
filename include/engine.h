
#include "game.h"
#include "definitions.h"
#include "evaluator.h"

class Engine
{
public:

    int depth;

    Color color;

    Game* game;

    Evaluator evaluator;

    Engine(Color color, Game* game, int depth);

    std::pair<Move, double> chooseMove(uint8_t &castleRights);

    int getMoveScore(const Move move);    

    void sortMoves(std::vector<Move> &moves);

    double search(int depth, double alpha, double beta, Color color, Move& bestMove, uint8_t castleRights, long long &counter);
};