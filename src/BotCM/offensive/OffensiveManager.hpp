#ifndef BOTCM_OFFENSIVE_MANAGER_HPP_
#define BOTCM_OFFENSIVE_MANAGER_HPP_

#include <vector>
#include <list>

#include "../CMEnums.hpp"
#include "../../Coordinates.hpp"
#include "PawnColor.hpp"
#include "../../Field.hpp"
#include "PotentialPawnSeries.hpp"
#include "../MovementCoordinatesWithGrade.h"
#include "PotentialCoordinates.hpp"

class Board;

namespace CM {

class OffensiveManager {
public:
    OffensiveManager();
    virtual ~OffensiveManager();

    void Initialize(const Board* const board, PawnColor playerColor, Field opponentPlayerColor);

    bool UpdatePotentialPawnSeriesAfterCurrentPlayerMovement(Coordinates& currentPlayerMovement);

    enum class DetermineBestOffensiveMovementResult {
		OffensiveMovementChosen,
		DefensiveMovementShallBeChosen,
		Error
	};

    bool DetermineBestOffensiveMovement(MovementCoordinatesWithGrade& movementCoordinatesWithGrade);

    void RemoveFromPotentialCoordinates(const Coordinates& outputCoordinates);

private:
    struct PotentialPawnSeriesData {
    	// Value 1 is the default, because the series count in each orientation equals always
    	// at least one because of the current pawn of the player.
        std::size_t currentPlayerPawnsInSeriesCount = 1u;

        std::list<Coordinates> currentPlayerPawnSeries;

        struct OneSideData {
        	std::size_t emptyFieldsCountBesidePawnSeries = 0u;
			std::size_t currentPlayerPawnsAfterEmptyFields = 0u;
			bool emptyFieldsAfterPawnSeriesAfterEmptyFields = false;
			Coordinates firstEmptyFieldCoordinates;
        } leftSideData, rightSideData;
    };

    bool updatePotentialPawnSeriesInOneOrientation(PawnSeriesOrientation pawnSeriesOrientation);
    void updatePotentialPawnSeriesOneSideData(PawnSeriesOrientation pawnSeriesOrientation, Monotonicity direction, PotentialPawnSeriesData& potentialPawnSeriesData);
    void AddPotentialPawnSeriesToList(std::vector<std::list<PotentialPawnSeries>>& potentialPawnSeriesList, PotentialPawnSeriesData& potentialPawnSeriesData, PawnSeriesOrientation pawnSeriesOrientation);
    void AddPotentialPawn3LongSeriesToListIfPotential(PotentialPawnSeriesData& potentialPawnSeriesData, PawnSeriesOrientation pawnSeriesOrientation);
    void AddPotentialPawn4LongSeriesToListIfPotential(PotentialPawnSeriesData& potentialPawnSeriesData, PawnSeriesOrientation pawnSeriesOrientation);

    PotentialCoordinates determinePotentialCoordinatesForOneSide(PotentialPawnSeriesData& potentialPawnSeriesData, PawnSeriesOrientation pawnSeriesOrientation, PotentialPawnSeriesData::OneSideData& currentSideData, PotentialPawnSeriesData::OneSideData& oppositeSideData);

    // This function could be called only for enlarging 2- or 3-long pawn series, by one pawn.
    // 'lenghtOfPawnSeriesToUpdate' - could equal only 2 or 3.
    // This function makes new potential pawn series if a corresponding shorter one is not found.
    bool updateOrCreateNewPotentialPawnSeries(PotentialPawnSeriesData& potentialPawnSeriesData, PawnSeriesOrientation pawnSeriesOrientation, std::size_t lenghtOfPawnSeriesToUpdate);

    // 'lenghtOfPawnSeriesToUpdate' - could equal only 2 or 3.
    bool enlargeExistingPotentialPawnSeries(
    		std::size_t lenghtOfPawnSeriesToUpdate,
    		std::list<Coordinates>& pawnSeries,
			std::size_t potentialPawnSeriesBeginningX,
			PotentialPawnSeriesData& potentialPawnSeriesData,
			End endOfEnlargingPotentialSeries,
			PawnSeriesOrientation pawnSeriesOrientation);

    bool determineOffensiveMovementInPawnSeriesList(
    		std::vector<std::list<PotentialPawnSeries>>& potentialPawnXLongSeriesList,
			MovementCoordinatesWithGrade& outputCoordinatesWithGrade,
			unsigned int potentialPawnSeriesListLength);

    bool checkIfCoordinatesAreStillPotential(PotentialCoordinates& potentialCoordinates);

    bool isFieldOnBoardAndIsEmpty(const Coordinates& coordinates);
    Coordinates determineNeighbourCoordinatesToCurrentPlayerMovement();

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

    // This list contains coordinates of only free fields, where placing a pawn by
    // current player will be very beneficial in the matted of offensive.
    std::list<PotentialCoordinates> potentialCoordinatesList;

    // Finally this constant should be set to false.
    const bool TreatNotFoundPotentialPawnSeriesToEnlargeAsError = false;

    const bool LogOffensiveData = true;
};

}

#endif
