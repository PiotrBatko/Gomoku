#include "GameController.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "Random.h"
#include "CommonUtils.h"
#include "AppConfig/FileAppConfigContainer.h"
#include "AppConfig/FileAppConfigLoader.h"
#include "CommonEnums.hpp"
#include "GameFinishedChecker.h"

#include "BotRandomizer.hpp"
#include "ConsolePlayer.hpp"
#include "BotCM/BotCM.hpp"
#include "BotPB/BotPB.hpp"

GameController::GameController() {

}

GameController::~GameController() {

}

bool GameController::Run() {
    bool result = Initialize();
    if (!result) {
        return false;
    }

    m_WhitePlayer = createPlayer(fileAppConfigContainer.PlayerWhite);
    m_BlackPlayer = createPlayer(fileAppConfigContainer.PlayerBlack);

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

        GameFinishedChecker gameFinishedChecker(m_Board);

        Field winner = Field::Empty;

        // Main game loop.
        // TODO: extract it to new function.
        while (!battleFinished) {

            // 1. White player's turn.
            Field currentPlayerColor = Field::White;
            Coordinates currentPlayerMove = makePlayerMove(m_WhitePlayer.get(), currentPlayerColor);
            m_BlackPlayer->NotifyAboutOpponentMove(currentPlayerMove);

            drawGameBoard();

            result = gameFinishedChecker.CheckIfGameFinished(currentPlayerMove, currentPlayerColor, battleFinished);
            if (!result) return false;
            if (battleFinished) {
                winner = currentPlayerColor;
                continue;
            }

            // 2. Black player's turn.
            currentPlayerColor = Field::Black;
            currentPlayerMove = makePlayerMove(m_BlackPlayer.get(), currentPlayerColor);
            m_WhitePlayer->NotifyAboutOpponentMove(currentPlayerMove);

            drawGameBoard();

            result = gameFinishedChecker.CheckIfGameFinished(currentPlayerMove, currentPlayerColor, battleFinished);
            if (!result) return false;
            if (battleFinished) {
                winner = currentPlayerColor;
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
    }

    // TODO: extract it to function.
    unsigned long long int allocationCounter = AllocationCounter::GetCounter();
    if (allocationCounter != 0uLL) {
        LOG_LN("Allocation counter is not zero. At least one 'new' has no corresponding 'delete'.");
        return false;
    }
    return true;
}

std::unique_ptr<Player> GameController::createPlayer(const int playerTypeId) {
    PlayerType playerType = static_cast<PlayerType>(playerTypeId);

    switch (playerType)
    {
        case PlayerType::HUMAN_CONSOLE:
            return std::make_unique<ConsolePlayer>(&m_Board);
        case PlayerType::BOT_RANDOMIZER:
            return std::make_unique<BotRandomizer>(&m_Board);
        case PlayerType::BOT_CM:
            return std::make_unique<CM::BotCM>(&m_Board);
        case PlayerType::BOT_PB:
            return std::make_unique<PB::BotPB>(&m_Board);
        default:
            throw std::runtime_error("Wrong player type id");
    }
}

bool GameController::Initialize() {
    Random::Initialize();

    FileAppConfigLoader fileAppConfigLoader;
    bool result = fileAppConfigLoader.LoadFileAppConfigConstants(fileAppConfigContainer);
    if (!result) {
        return false;
    }

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
                                           const Field field) {
    Coordinates move = player->MakeMove();

    // Ensure that movement made by player is valid.
    bool result = false;
    bool isFieldEmpty = m_Board.IsFieldEmpty(move.x, move.y, result);
    if (!result || !isFieldEmpty) {
        throw std::runtime_error("Unnamed error");
    }

    // Set player's movement on the board.
    result = m_Board.SetField(move.x, move.y, field);
    if (!result) {
        throw std::runtime_error("Unnamed error");
    }
    return move;
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
