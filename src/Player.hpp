#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <utility>

#include "Coordinates.hpp"
#include "CommonEnums.hpp"
#include "PawnColor.hpp"

class Board;

class Player
{
public:

    Player(const Board* gameBoard, PlayerType playerType, PawnColor playerColor);
    virtual ~Player() = default;

    /// Player is notified after opponent's move. It is good moment
    /// to update some local informations about game progress.
    virtual void NotifyAboutOpponentMove(Coordinates opponentMove) = 0;

    virtual Coordinates MakeMove() = 0;

    /// Player is notified that view requested a move. Request may be ignored,
    /// (this is default implementation behavior), but also can be used to
    /// support decision about next move.
    virtual void NotifyAboutRequestedMove(Coordinates requestedMove);

    PlayerType GetPlayerType();
    PawnColor GetColor() const;
    bool IsPlayerBot() const;
    bool IsPlayerHuman() const;

protected:

    const Board* const m_Board;

    const PlayerType m_PlayerType;
    const PawnColor m_PlayerColor;
};

#endif
