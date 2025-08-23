
#include "game.hpp"
#include "definitions.hpp"
#include "evaluator.hpp"
#include <chrono>

namespace MoveBonuses {
    constexpr int CAPTURE = 100;
    constexpr int CHECK = 100;
    constexpr int CASTLE = 50;
}

typedef std::chrono::system_clock::time_point time_point_t;

class Engine
{
public:

    uint minDepth;
    uint maxDepth;
    double thinkingTime = 20;

    Color color;

    Game* game;

    Evaluator evaluator;

    Engine(Color color, Game* game, uint minDepth, uint maxDepth, double thinkingTime);

    std::pair<Move, int> chooseMove(uint8_t &castleRights);

    int getMoveScore(Move &move);    

    void sortMoves(std::vector<Move> &moves);

    bool timeUp(time_point_t start);

    int search(int depth, int alpha, int beta, Color color, Move& bestMove, uint8_t castleRights, long long &counter, time_point_t timerStart);

    int quiescence(int alpha, int beta, Color color, uint8_t castleRights, long long &counter, time_point_t timerStart);
};