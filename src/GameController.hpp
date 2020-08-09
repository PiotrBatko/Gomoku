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

    Coordinates makePlayerMove(Player* const player, const Field field, PlayerMovementStatus& playerMovementStatus);

    bool Initialize();

    std::unique_ptr<Player> createPlayer(const int playerTypeId, Field playerColor);

    bool processPlayerTurn(
            const Field currentPlayerColor,
            const Field notCurrentPlayerColor,
            const std::unique_ptr<Player>& currentPlayer,
            const std::unique_ptr<Player>& notCurrentPlayer,
            bool& battleFinished,
            Field& winner,
            FinishCause& battleFinishCause);

    void waitForEnterKeyIfNeeded();

    Board m_Board;

    std::unique_ptr<Player> m_WhitePlayer;
    std::unique_ptr<Player> m_BlackPlayer;

    GameFinishedChecker m_GameFinishedChecker;

    bool m_ShouldRun = false;

    std::vector<GameView*> m_Views;
};

#endif
