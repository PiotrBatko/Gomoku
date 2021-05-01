#ifndef BOTCM_OFFENSIVE_MANAGER_HPP_
#define BOTCM_OFFENSIVE_MANAGER_HPP_

#include <vector>
#include <list>

#include "../CMEnums.hpp"
#include "../../Coordinates.hpp"
#include "PawnColor.hpp"
#include "../../Field.hpp"
#include "PotentialPawnSeries.hpp"

class Board;

namespace CM {

class OffensiveManager {
public:
    OffensiveManager();
    virtual ~OffensiveManager();

    void Initialize(const Board* const board, PawnColor playerColor, Field opponentPlayerColor);

    bool UpdatePotentialPawnSeriesAfterCurrentPlayerMovement(Coordinates& currentPlayerMovement);


private:
    struct PotentialPawnSeriesData {
    	// Value 1 is the default, because the series count in each orientation equals always
    	// at least one because of the current pawn of the player.
        std::size_t currentPlayerPawnsInSeriesCount = 1u;

        std::list<Coordinates> currentPlayerPawnSeries;

        // Count of empty fields on left (lower)/right (higher) fields from current player pawn series.
        std::size_t emptyFieldsOnLeftSide  = 0u;
        std::size_t emptyFieldsOnRightSide = 0u;
    };

    bool updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation);
    void updatePotentialPawnSeriesOneSideData(PawnSeriesOrientation pawnSeriesOrientation, Monotonicity direction, PotentialPawnSeriesData& potentialPawnSeriesData);
    void AddPotentialPawnSeriesToList(std::vector<std::list<PotentialPawnSeries>>& potentialPawnSeriesList, PotentialPawnSeriesData& potentialPawnSeriesData);
    void AddPotentialPawn3LongSeriesToListIfPotential(PotentialPawnSeriesData& potentialPawnSeriesData);
    void AddPotentialPawn4LongSeriesToListIfPotential(PotentialPawnSeriesData& potentialPawnSeriesData);

    // This function could be called only for enlarging 2- or 3-long pawn series, by one pawn.
    // 'lenghtOfPawnSeriesToUpdate' - could equal only 2 or 3.
    bool updatePotentialPawnSeries(PotentialPawnSeriesData& potentialPawnSeriesData, std::size_t lenghtOfPawnSeriesToUpdate);

    // 'lenghtOfPawnSeriesToUpdate' - could equal only 2 or 3.
    bool enlargeExistingPotentialPawnSeries(
    		std::size_t lenghtOfPawnSeriesToUpdate,
    		std::list<Coordinates>& pawnSeries,
			std::size_t potentialPawnSeriesBeginningX,
			PotentialPawnSeriesData& potentialPawnSeriesData,
			End endOfEnlargingPotentialSeries);

    Coordinates currentPlayerMovement;

    const Board* board;
    PawnColor playerColor;
    Field opponentPlayerColor;

    // For each collection below:
    // First dimension - column index of starting pawn of given potential series
    // Second dimension - collection of pawn series which starting pawn has given column index.
    std::vector<std::list<PotentialPawnSeries>> potentialPawn4LongSeriesList;
    std::vector<std::list<PotentialPawnSeries>> potentialPawn3LongSeriesList;
    std::vector<std::list<PotentialPawnSeries>> potentialPawn2LongSeriesList;

    // Finally this constant should be set to false.
    const bool TreatNotFoundPotentialPawnSeriesToEnlargeAsError = false;

    const bool LogOffensiveData = true;
};

}

#endif
