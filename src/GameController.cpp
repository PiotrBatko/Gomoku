#include "GameController.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "AppConfig/FileAppConfigContainer.hpp"
#include "Board.hpp"
#include "CommonEnums.hpp"
#include "DebugInfo.hpp"
#include "GameFinishedChecker.hpp"
#include "Random.hpp"

#include "BatoBot/BatoBot.hpp"
#include "BotCM/BotCM.hpp"
#include "BotRandomizer.hpp"
#include "ConsolePlayer.hpp"

GameController::GameController() :
    m_GameFinishedChecker(m_Board)
{
}

GameController::~GameController() {
}

void GameController::Terminate()
{
    m_ShouldRun = false;
}

bool GameController::Run() {
    bool result = Initialize();
    if (!result) {
        return false;
    }

    m_ShouldRun = true;

    for (auto view : m_Views)
    {
        view->GameStarted(m_Board.getWidth(), m_Board.getHeight());
    }

    m_WhitePlayer = createPlayer(fileAppConfigContainer.PlayerWhite, PawnColor::White);
    m_BlackPlayer = createPlayer(fileAppConfigContainer.PlayerBlack, PawnColor::Black);

    bool battleFinished = false;

    Field winner = Field::Empty;
    FinishCause battleFinishCause = FinishCause::None;

    // Main game loop.
    // TODO: extract it to new function.
    while (!battleFinished) {

        // 1. White player's turn.
        result = processPlayerTurn(
                Field::White,
                Field::Black,
                m_WhitePlayer,
                m_BlackPlayer,
                battleFinished,
                winner,
                battleFinishCause);
        if (!result) {
            return false;
        }
        if (battleFinished) {
            continue;
        }

        // 2. Black player's turn.
        result = processPlayerTurn(
                Field::Black,
                Field::White,
                m_BlackPlayer,
                m_WhitePlayer,
                battleFinished,
                winner,
                battleFinishCause);
        if (!result) {
            return false;
        }
    }

    // TODO: extract it to function.
    std::string winnerColor;
    if (winner == Field::White) {
        winnerColor = "White";
    } else {
        winnerColor = "Black";
    }
    LOG_LN("Game finished. The winner is ", winnerColor, " player. Congratulations!");
    switch (battleFinishCause) {
        case FinishCause::EnoughPlayerPawnsInLine:
            LOG_LN("The cause of finish was enough pawns in line.");
            break;
        case FinishCause::PlayerTurnMaxTimeExceeded:
            LOG_LN("The cause of finish: player turn maximum time exceeded.");
            break;
        case FinishCause::WrongMovement:
            LOG_LN("The cause of finish: player made an invalid movement.");
            break;
        default:
            LOG_ERROR("Wrong value of 'battleFinishCause'.");
            return false;
    }

    for (auto view : m_Views)
    {
        view->GameFinished();
    }

    while (m_ShouldRun)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);
    }

    for (auto view : m_Views)
    {
        view->Terminate();
    }

    return true;
}

void GameController::RegisterView(GameView& gameView)
{
    m_Views.push_back(&gameView);
    gameView.SetGameModel(*this);
}

bool GameController::processPlayerTurn(
        const Field currentPlayerColor,
        const Field notCurrentPlayerColor,
        const std::unique_ptr<Player>& currentPlayer,
        const std::unique_ptr<Player>& notCurrentPlayer,
        bool& battleFinished,
        Field& winner,
        FinishCause& battleFinishCause) {

    const auto startTime = std::chrono::high_resolution_clock::now();

    PlayerMovementStatus playerMovementStatus;
    Coordinates currentPlayerMove = makePlayerMove(currentPlayer.get(), currentPlayerColor, playerMovementStatus);

    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count();

    PawnColor color = currentPlayerColor == Field::White ? PawnColor::White : PawnColor::Black;
    for (auto view : m_Views)
    {
        view->PawnAdded(color, currentPlayerMove);
    }

    switch (playerMovementStatus) {
        case PlayerMovementStatus::ValidMovement:
            break;
        case PlayerMovementStatus::WrongMovement:
            battleFinished = true;
            winner = notCurrentPlayerColor;
            battleFinishCause = FinishCause::WrongMovement;
            return true;
        default:
            LOG_ERROR("Error occurred.");
    }

    notCurrentPlayer->NotifyAboutOpponentMove(currentPlayerMove);

    // If current player is not a console human player, he/she has limited time to perform the movement.
    if (currentPlayer->GetPlayerType() != PlayerType::HUMAN_CONSOLE) {
        const int playerTurnMaxTime = fileAppConfigContainer.PlayerTurnMaxTime;
        const bool isPlayerTurnTimeLimitDisabled = (playerTurnMaxTime == 0);

        if (!isPlayerTurnTimeLimitDisabled) {
            const int elapsedTimeInt = static_cast<int>(elapsedTime);

            if (elapsedTimeInt > playerTurnMaxTime) {
                battleFinished = true;
                winner = notCurrentPlayerColor;
                battleFinishCause = FinishCause::PlayerTurnMaxTimeExceeded;
                return true;
            }
        }
    }

    const bool result = m_GameFinishedChecker.CheckIfGameFinished(currentPlayerMove, currentPlayerColor, battleFinished);
    if (!result) {
        return false;
    }
    if (battleFinished) {
        winner = currentPlayerColor;
        battleFinishCause = FinishCause::EnoughPlayerPawnsInLine;
        return true;
    }
    waitForEnterKeyIfNeeded();
    return true;
}

std::unique_ptr<Player> GameController::createPlayer(const int playerTypeId, const PawnColor playerColor) {
    PlayerType playerType = static_cast<PlayerType>(playerTypeId);

    switch (playerType)
    {
        case PlayerType::HUMAN_CONSOLE:
            return std::make_unique<ConsolePlayer>(&m_Board, playerType, playerColor);
        case PlayerType::BOT_RANDOMIZER:
            return std::make_unique<BotRandomizer>(&m_Board, playerType, playerColor);
        case PlayerType::BOT_CM:
            return std::make_unique<CM::BotCM>(&m_Board, playerType, playerColor);
        case PlayerType::BOT_BATOBOT:
            return std::make_unique<batobot::BatoBot>(&m_Board, playerType, playerColor);
        default:
            throw std::runtime_error("Wrong player type id");
    }
}

bool GameController::Initialize() {
    Random::Initialize();

    const std::size_t BoardSize = static_cast<std::size_t>(fileAppConfigContainer.BoardSize);
    m_Board.SetSize(BoardSize, BoardSize);

    return true;
}

Coordinates GameController::makePlayerMove(Player* const player,
                                           const Field field,
                                           PlayerMovementStatus& playerMovementStatus) {

    const Coordinates movement = player->MakeMove();

    // Ensure that movement made by player is valid.
    bool result = false;
    bool isFieldEmpty = m_Board.IsFieldEmpty(movement.x, movement.y, result, true);
    if (!result || !isFieldEmpty) {
        playerMovementStatus = PlayerMovementStatus::WrongMovement;
        return Coordinates();
    }

    // Set player's movement on the board.
    result = m_Board.SetField(movement.x, movement.y, field);
    if (!result) {
        playerMovementStatus = PlayerMovementStatus::Error;
    }

    playerMovementStatus = PlayerMovementStatus::ValidMovement;
    return movement;
}

void GameController::waitForEnterKeyIfNeeded() {
    const int humanConsolePlayerId = static_cast<int>(PlayerType::HUMAN_CONSOLE);

    // Ensure that both players are not human players.
    if (   fileAppConfigContainer.PlayerWhite != humanConsolePlayerId
        && fileAppConfigContainer.PlayerBlack != humanConsolePlayerId) {

        if (fileAppConfigContainer.EnterPressedNeededToMakeNextMove) {
            // There is getchar() function used because it finishes just after pressing enter key
            // when "cin >> value" needs other key to be pressed before enter key.
            getchar();
        }
    }
}
