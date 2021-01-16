#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <atomic>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "Board.hpp"
#include "Coordinates.hpp"
#include "Field.hpp"
#include "GameFinishedChecker.hpp"
#include "GameModel.hpp"
#include "GameView.hpp"
#include "Player.hpp"

class Player;

class GameController :
    public GameModel
{
public:

    GameController();
    ~GameController();

    void MakeMove(Coordinates coordinates) override;
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

    struct GameResult
    {
        FinishCause m_FinishCause;
        PawnColor m_WinnerColor;
    };

    Coordinates makePlayerMove(Player* const player, PlayerMovementStatus& playerMovementStatus);

    bool Initialize();

    std::unique_ptr<Player> createPlayer(const int playerTypeId, PawnColor playerColor);

    std::optional<GameResult> ProcessPlayerTurn();

    void waitForEnterKeyIfNeeded();

    void SwitchNextPlayerTurn();

    Board m_Board;

    std::unique_ptr<Player> m_WhitePlayer;
    std::unique_ptr<Player> m_BlackPlayer;
    Player* m_CurrentPlayer = nullptr;
    Player* m_OppositePlayer = nullptr;

    GameFinishedChecker m_GameFinishedChecker;

    std::atomic<bool> m_ShouldRun = false;

    std::vector<GameView*> m_Views;
};

#endif
