
#ifndef SRC_BOTCM_CMENUMS_HPP_
#define SRC_BOTCM_CMENUMS_HPP_

namespace CM {

// Orientation of possible pawn series to win.
enum class PawnSeriesOrientation {
    // Column is not changing, rows are incrementing.
    VERTICAL,
    // Row is not changing, columns are incrementing.
    HORIZONTAL,
    // Columns are incrementing, rows are decrementing.
    INCREASING,
    // Columns are incrementing and rows are incrementing too.
    DECREASING
};

enum class Monotonicity {
    DECREASING,
    INCREASING
};

enum class End {
    FRONT,
    BACK
};

}

#endif
