#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "Coordinates.hpp"

/// Game view sends commands via this interface to model
class GameModel
{
public:

    /// Invoked when move is requested.
    virtual void MakeMove(Coordinates coordinates) = 0;

    /// Invoked when model should be terminated.
    virtual void Terminate() = 0;

    virtual ~GameModel() = default;
};

#endif
