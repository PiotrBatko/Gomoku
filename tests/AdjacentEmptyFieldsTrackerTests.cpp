#include "catch.hpp"

#include "BatoBot/Engine/AdjacentEmptyFieldsTracker.hpp"

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
    }
}
