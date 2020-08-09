#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <memory>
#include <utility>
#include <vector>

#include "Board.hpp"
#include "Coordinates.hpp"
#include "Field.hpp"
#include "GameFinishedChecker.hpp"
#include "GameModel.hpp"
#include "GameView.hpp"

class Player;

class GameController :
    public GameModel
{
public:

    GameController();
    virtual ~GameController();

    void Terminate() override;

    bool Run();

    void RegisterView(GameView& gameView);

private:

    enum class FinishCause {
        None,
        EnoughPlayerPawnsInLine,
        PlayerTurnMaxTimeExceeded,
        WrongMovement
    };

    enum class PlayerMovementStatus {
        None,
        ValidMovement,
        WrongMovement,
        Error
    };

    Coordinates makePlayerMove(Player* const player, PlayerMovementStatus& playerMovementStatus);

    bool Initialize();

    std::unique_ptr<Player> createPlayer(const int playerTypeId, PawnColor playerColor);

    bool processPlayerTurn(
            bool& battleFinished,
            PawnColor& winner,
            FinishCause& battleFinishCause);

    void waitForEnterKeyIfNeeded();

    void SwitchNextPlayerTurn();

    Board m_Board;

    std::unique_ptr<Player> m_WhitePlayer;
    std::unique_ptr<Player> m_BlackPlayer;
    Player* m_CurrentPlayer = nullptr;
    Player* m_OppositePlayer = nullptr;

    GameFinishedChecker m_GameFinishedChecker;

    bool m_ShouldRun = false;

    std::vector<GameView*> m_Views;
};

#endif
