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
                const auto& trackedFields = tracker.GetAdjacentFields();

                REQUIRE(trackedFields.size() == 8);
            }
        }
    }
}
