#ifndef GAMEFINISHEDCHECKER_HPP
#define GAMEFINISHEDCHECKER_HPP

#include <utility>

#include "Coordinates.hpp"
#include "PawnColor.hpp"

class Board;

class GameFinishedChecker
{
public:

    GameFinishedChecker(const Board& board);
    virtual ~GameFinishedChecker();

    bool CheckIfGameFinished(Coordinates lastPlayerMovement,
                             PawnColor lastPlayerColor,
                             bool& checkingResult);

private:

    // This function checks non-slanting lines (one horizontal and one vertical)
    // in both directions.
    void processNonSlantingLines(bool& checkingResult);

    // This function checks slanting lines in both directions.
    void processSlantingLines(bool& checkingResult);

    // Value of parameter 'revertGFirst':
    // false - function checks slanting line from top left to bottom right,
    // true  - function checks slanting line from top right to bottom left,
    // True as returned value means that player has won.
    bool checkSlantingLine(bool negateIForColumns);

    std::size_t lastPlayerMovementX;
    std::size_t lastPlayerMovementY;

    PawnColor lastPlayerColor = PawnColor::White;

    const Board& board;
    std::size_t pawnsLineLenghtToWin;

    std::size_t pawnsInLineCounter;

    int lastPlayerMovementX_Int;
    int lastPlayerMovementY_Int;

    // These two variables are used for checking slanting lines.
    int iFirst;
    int iLast;

    // Symbolic name for 'pawnsLineLenghtToWin' minus 1.
    std::size_t K;

    std::size_t boardSize;
};

#endif
