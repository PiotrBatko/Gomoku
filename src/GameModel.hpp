#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

/// Game model interface for views
class GameModel
{
public:

    /// Invoked when model should be terminated.
    virtual void Terminate() = 0;

    virtual ~GameModel() = default;
};

#endif
