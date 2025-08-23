
#pragma once

#include<vector>
#include "definitions.hpp"
#include<string>
#include<algorithm>
#include<cstdint>

bool isWhite(Piece piece);

bool isSameColor(Piece a, Piece b);

Color getColor(Piece p);

int algebraicNotationToPosition(std::string notation);

std::string positionToAlgebraic(int pos);

template <typename T>
bool contains(const std::vector<T>& vec, const T& value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

int getPieceValue(Piece piece);

Color opponentColor(Color color);

std::string pieceToName(Piece p);

std::string getColorName(Color color);

std::vector<int> findPieces(Piece board[120], Piece value);

char pieceToSymbol(Piece piece);

int randInt(int start, int end);

std::string castleRightsToString(uint8_t flags);

template <typename T>
void replace(T arr[], int size, T x, T y) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == x) {
            arr[i] = y;
        }
    }
}

std::string debugMove(const Move m);