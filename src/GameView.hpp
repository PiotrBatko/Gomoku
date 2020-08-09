#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include <cstddef>

#include "GameModel.hpp"

/// Game model interface for views
class GameView
{
public:

    /// Invoked at the beginning of each game. Notifies view about
    /// board dimensions.
    virtual void GameStarted(std::size_t numberOfColumns, std::size_t numberOfRows) = 0;

    /// Invoked at the end of each game. Game may be finished because one player
    /// set 5 pawns in a row, time runs out, players accept draw or maybe even
    /// something else. Please not confuse this event with Terminate.
    virtual void GameFinished() = 0;

    /// Invoked when view should be terminated.
    virtual void Terminate() = 0;

    virtual ~GameView() = default;

    void SetGameModel(GameModel& model);

protected:

    GameModel& GetGameModel() const;

private:

    GameModel* m_Model;
};

#endif
