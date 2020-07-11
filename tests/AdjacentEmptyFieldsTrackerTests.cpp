#include "catch.hpp"

#include "BatoBot/Engine/AdjacentEmptyFieldsTracker.hpp"

#include <algorithm>

using namespace batobot;

namespace
{

constexpr std::size_t BoardWidth = 5;
constexpr std::size_t BoardHeight = 5;

}

SCENARIO ("Tracker returns adjacent empty fields", "[AdjacentEmptyFieldsTracker]")
{
    GIVEN ("Brand new tracker")
    {
        AdjacentEmptyFieldsTracker tracker(BoardWidth, BoardHeight);

        REQUIRE(tracker.GetAdjacentFields().empty());

        WHEN ("Single move is registered")
        {
            tracker.RegisterMove(Coordinates(2, 2));

            THEN ("All surrounding fields are tracked")
            {
                using C = Coordinates;
                const std::vector<Coordinates> RequiredTrackedFields = {
                    C(1, 1), C(2, 1), C(3, 1),
                    C(1, 2), /*move*/ C(3, 2),
                    C(1, 3), C(2, 3), C(3, 3)
                };

                REQUIRE(tracker.GetAdjacentFields() == RequiredTrackedFields);
            }
        }

        WHEN ("Two moves are registered")
        {
            tracker.RegisterMove(Coordinates(2, 2));
            tracker.RegisterMove(Coordinates(3, 3));

            THEN ("All surrounding fields are tracked")
            {
                using C = Coordinates;
                const std::vector<Coordinates> RequiredTrackedFields = {
                    C(1, 1), C(2, 1), C(3, 1),
                    C(1, 2), /*move*/ C(3, 2), C(4, 2),
                    C(1, 3), C(2, 3), /*move*/ C(4, 3),
                             C(2, 4), C(3, 4), C(4, 4)
                };

                REQUIRE(tracker.GetAdjacentFields() == RequiredTrackedFields);
            }
        }

        WHEN ("Move is done in top-left corner")
        {
            tracker.RegisterMove(Coordinates(0, 0));

            THEN ("All surrounding fields are tracked")
            {
                using C = Coordinates;
                const std::vector<Coordinates> RequiredTrackedFields = {
                    /*move*/ C(1, 0),
                    C(0, 1), C(1, 1)
                };

                REQUIRE(tracker.GetAdjacentFields() == RequiredTrackedFields);
            }
        }
    }
}
