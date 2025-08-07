
#include "definitions.h"

class Engine
{
public:

    Color color;

    Engine(Color color);

    Move chooseMove(Piece board[120]);
};