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

    virtual ~GameView() = default;

    void SetGameModel(GameModel& model);

protected:

    GameModel& GetGameModel() const;

private:

    GameModel* m_Model;
};

#endif
