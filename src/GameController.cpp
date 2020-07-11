#include "GameController.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "AppConfig/FileAppConfigContainer.h"
#include "Board.hpp"
#include "CommonEnums.hpp"
#include "DebugInfo.hpp"
#include "GameFinishedChecker.hpp"
#include "Random.hpp"
#include "CommonUtils.h"

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

bool GameController::Run() {
    bool result = Initialize();
    if (!result) {
        return false;
    }

    m_WhitePlayer = createPlayer(fileAppConfigContainer.PlayerWhite, Field::White);
    m_BlackPlayer = createPlayer(fileAppConfigContainer.PlayerBlack, Field::Black);

    bool battleFinished = false;

    // Main window loop.
    // TODO: extract it to new function.
    while (m_Window.isOpen())
    {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_Window.close();
            }
        }

        if (battleFinished) {
            continue;
        }

        // Initial drawing of the game board, for the first player to see the board.
        drawGameBoard();

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
    }

    result = verifyAllocationCounter();
    return result;
}

bool GameController::verifyAllocationCounter() {
    unsigned long long int allocationCounter = AllocationCounter::GetCounter();
    if (allocationCounter != 0uLL) {
        LOG_LN("Allocation counter is not zero. At least one 'new' has no corresponding 'delete'.");
        return false;
    }
    return true;
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
    drawGameBoard();

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

std::unique_ptr<Player> GameController::createPlayer(const int playerTypeId, const Field playerColor) {
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

    m_Window.create(
        sf::VideoMode(FieldWidthInPixels * m_Board.getWidth(), FieldHeightInPixels * m_Board.getHeight()),
        "Gomoku Bot Battle"
    );

    m_Font.loadFromFile("res/fonts/Ubuntu-L.ttf");

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

void GameController::drawGameBoard() {
    // Prepare board view
    constexpr float FieldPadding = 1.0F;
    constexpr float BlockRadius = FieldWidthInPixels / 2 - 2 * FieldPadding;

    sf::CircleShape whiteBlockView(BlockRadius);
    whiteBlockView.setFillColor(sf::Color(240, 240, 240));
    whiteBlockView.setOrigin(BlockRadius, BlockRadius);

    sf::CircleShape blackBlockView(BlockRadius);
    blackBlockView.setFillColor(sf::Color(16, 16, 16));
    blackBlockView.setOrigin(BlockRadius, BlockRadius);

    sf::RectangleShape fieldView(sf::Vector2f(FieldWidthInPixels, FieldHeightInPixels));
    fieldView.setFillColor(sf::Color(176, 144, 90));
    fieldView.setOutlineThickness(-0.5F);
    fieldView.setOutlineColor(sf::Color(214, 177, 114));
    fieldView.setOrigin(FieldWidthInPixels / 2, FieldHeightInPixels / 2);

    sf::Text fieldCoordinatesView;
    fieldCoordinatesView.setCharacterSize(10);
    fieldCoordinatesView.setFont(m_Font);
    fieldCoordinatesView.setFillColor(sf::Color(234, 193, 128));

    for (std::size_t y = 0u; y < m_Board.getHeight(); ++y)
    {
        for (std::size_t x = 0u; x < m_Board.getWidth(); ++x)
        {
            sf::Vector2f fieldCenter(
                FieldWidthInPixels / 2 + x * FieldWidthInPixels,
                FieldHeightInPixels / 2 + y * FieldHeightInPixels
            );
            fieldView.setPosition(fieldCenter);

            m_Window.draw(fieldView);

            if (m_Board.at(x, y) == Field::White)
            {
                whiteBlockView.setPosition(fieldCenter);
                m_Window.draw(whiteBlockView);
            }
            else if (m_Board.at(x, y) == Field::Black)
            {
                blackBlockView.setPosition(fieldCenter);
                m_Window.draw(blackBlockView);
            }
            else
            {
                std::ostringstream coordinates;
                coordinates << x << "\n" << y;

                sf::Vector2f fieldTopLeft(x * FieldWidthInPixels, y * FieldHeightInPixels);
                sf::Vector2f fieldCoordinatesMargin(3, 1);
                fieldCoordinatesView.setPosition(fieldTopLeft + fieldCoordinatesMargin);
                fieldCoordinatesView.setString(coordinates.str());
                m_Window.draw(fieldCoordinatesView);
            }
        }
    }

    m_Window.display();
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
