

#include<vector>
#include "definitions.h"
#include<string>
#include<algorithm>

bool isWhite(Piece piece);

bool isSameColor(Piece a, Piece b);

Color getColor(Piece p);

int algebraicNotationToPosition(std::string notation);

std::string positionToAlgebraic(int pos);

template <typename T>
bool contains(const std::vector<T>& vec, const T& value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}