#include "GameController.hpp"

#include <algorithm>
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
#include "UserControlledPlayer.hpp"

// For testing purposes:
#include "BotCM/InitialFieldCapturer.hpp"

GameController::GameController() :
    m_GameFinishedChecker(m_Board)
{
}

GameController::~GameController()
{
    for (auto view : m_Views)
    {
        view->Terminate();
    }
}

void GameController::MakeMove(Coordinates coordinates)
{
    m_CurrentPlayer->NotifyAboutRequestedMove(coordinates);
}

void GameController::Terminate()
{
    m_ShouldRun = false;
}

bool GameController::Run()
{
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

    //FOR TESTING:
    CM::InitialFieldCapturer initialFieldCapturer;
    result = initialFieldCapturer.Run(m_Board, m_BlackPlayer.get(), m_Views);
    if (!result) {
        return false;
    }
    //End of testing code.

    m_CurrentPlayer = m_WhitePlayer.get();
    m_OppositePlayer = m_BlackPlayer.get();

    // Main game loop.
    std::optional<GameResult> gameResult;
    while (not gameResult.has_value())
    {
        gameResult = ProcessPlayerTurn();

        if (not m_ShouldRun)
        {
            return true;
        }

        SwitchNextPlayerTurn();
    }

    // TODO: extract it to function.
    std::string winnerColor;
    if (gameResult->m_WinnerColor == PawnColor::White)
    {
        winnerColor = "White";
    }
    else
    {
        winnerColor = "Black";
    }

    LOG_LN("Game finished. The winner is ", winnerColor, " player. Congratulations!");
    switch (gameResult->m_FinishCause)
    {
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

    return true;
}

void GameController::RegisterView(GameView& gameView)
{
    m_Views.push_back(&gameView);
    gameView.SetGameModel(*this);
}

std::optional<GameController::GameResult> GameController::ProcessPlayerTurn()
{
    const auto startTime = std::chrono::high_resolution_clock::now();

    PlayerMovementStatus playerMovementStatus;
    Coordinates currentPlayerMove = makePlayerMove(m_CurrentPlayer, playerMovementStatus);

    if (not m_ShouldRun)
    {
        return std::nullopt;
    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count();

    for (auto view : m_Views)
    {
        view->PawnAdded(m_CurrentPlayer->GetColor(), currentPlayerMove);
    }

    switch (playerMovementStatus) {
        case PlayerMovementStatus::ValidMovement:
            break;
        case PlayerMovementStatus::WrongMovement:
        {
            return GameResult{
                FinishCause::WrongMovement,
                m_OppositePlayer->GetColor()
            };
        }
        default:
            LOG_ERROR("Error occurred.");
    }

    m_OppositePlayer->NotifyAboutOpponentMove(currentPlayerMove);

    // If current player is not a console human player, he/she has limited time to perform the movement.
    if (m_CurrentPlayer->GetPlayerType() != PlayerType::HUMAN_CONSOLE) {
        const int playerTurnMaxTime = fileAppConfigContainer.PlayerTurnMaxTime;
        const bool isPlayerTurnTimeLimitDisabled = (playerTurnMaxTime == 0);

        if (!isPlayerTurnTimeLimitDisabled) {
            const int elapsedTimeInt = static_cast<int>(elapsedTime);

            if (elapsedTimeInt > playerTurnMaxTime) {
                return GameResult{
                    FinishCause::PlayerTurnMaxTimeExceeded,
                    m_OppositePlayer->GetColor()
                };
            }
        }
    }

    bool battleFinished;
    const bool result = m_GameFinishedChecker.CheckIfGameFinished(currentPlayerMove, m_CurrentPlayer->GetColor(), battleFinished);
    if (!result) {
        throw std::runtime_error("Unnamed error");
    }
    if (battleFinished) {
        return GameResult{
            FinishCause::EnoughPlayerPawnsInLine,
            m_OppositePlayer->GetColor()
        };
    }
    waitForEnterKeyIfNeeded();
    return std::nullopt;
}

std::unique_ptr<Player> GameController::createPlayer(const int playerTypeId, const PawnColor playerColor) {
    PlayerType playerType = static_cast<PlayerType>(playerTypeId);

    switch (playerType)
    {
        case PlayerType::HUMAN_CONSOLE:
            return std::make_unique<ConsolePlayer>(&m_Board, playerType, playerColor);
        case PlayerType::HUMAN:
            return std::make_unique<UserControlledPlayer>(&m_Board, playerType, playerColor, m_ShouldRun);
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
                                           PlayerMovementStatus& playerMovementStatus) {

    const Coordinates movement = player->MakeMove();

    if (not m_ShouldRun)
    {
        return movement;
    }

    // Ensure that movement made by player is valid.
    bool result = false;
    bool isFieldEmpty = m_Board.IsFieldEmpty(movement.x, movement.y, result, true);
    if (!result || !isFieldEmpty) {
        playerMovementStatus = PlayerMovementStatus::WrongMovement;
        return Coordinates();
    }

    // Set player's movement on the board.
    result = m_Board.SetField(movement.x, movement.y, player->GetColor());
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

void GameController::SwitchNextPlayerTurn()
{
    std::swap(m_CurrentPlayer, m_OppositePlayer);
}
