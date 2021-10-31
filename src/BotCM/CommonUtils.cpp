#include "CommonUtils.hpp"

#include <cmath>

#include "../Coordinates.hpp"
#include "../Board.hpp"

namespace CM {

bool IsFieldOnBoardAndIsEmpty(const Coordinates& coordinates, const Board* const board) {
    bool isFieldOnBoard;
    bool isFieldEmpty = board->IsFieldEmpty(coordinates.x, coordinates.y, isFieldOnBoard, false);
    if (isFieldOnBoard && isFieldEmpty) {
        return true;
    }
    return false;
}

std::size_t CalculateCoordinatesDistance(const Coordinates& a, const Coordinates& b) {
    const Coordinates distanceInCoordinates = Coordinates(abs(a.x - b.x), abs(a.y - b.y));
    const std::size_t distanceInNumber = static_cast<std::size_t>(sqrt(distanceInCoordinates.x*distanceInCoordinates.x + distanceInCoordinates.y*distanceInCoordinates.y));
    return distanceInNumber;
}

}
