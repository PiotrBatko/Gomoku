#include "GameController.hpp"

#include <iostream>
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

	const std::size_t BoardSize = static_cast<std::size_t>(fileAppConfigContainer.BoardSize);
	Board board(BoardSize, BoardSize);

	constexpr std::size_t FieldWidthInPixels = 25u;
	constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;
	sf::RenderWindow window(
		sf::VideoMode(FieldWidthInPixels * board.getWidth(), FieldHeightInPixels * board.getHeight()),
		"Gomoku Bot Battle"
	);

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

	Player* playerWhite = nullptr;
	Player* playerBlack = nullptr;

	result = createPlayer(fileAppConfigContainer.PlayerWhite, playerWhite, board);
	if (!result) return false;
	result = createPlayer(fileAppConfigContainer.PlayerBlack, playerBlack, board);
	if (!result) return false;

	bool battleFinished = false;

	// Main window loop.
	// TODO: extract it to new function.
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		if (battleFinished) {
			continue;
		}

		// Initial drawing of the game board, for the first player to see the board.
		drawGameBoard(board, FieldWidthInPixels, FieldHeightInPixels, fieldView, window, whiteBlockView, blackBlockView);

		GameFinishedChecker gameFinishedChecker(board);

		Field winner = Field::Empty;

		// Main game loop.
		// TODO: extract it to new function.
		while (!battleFinished) {

			// 1. White player's turn.
			Field currentPlayerColor = Field::White;
			Coordinates currentPlayerMove = makePlayerMove(board, playerWhite, currentPlayerColor);
			playerBlack->NotifyAboutOpponentMove(currentPlayerMove);

			drawGameBoard(board, FieldWidthInPixels, FieldHeightInPixels, fieldView, window, whiteBlockView, blackBlockView);

			result = gameFinishedChecker.CheckIfGameFinished(currentPlayerMove, currentPlayerColor, battleFinished);
			if (!result) return false;
			if (battleFinished) {
				winner = currentPlayerColor;
				continue;
			}

			// 2. Black player's turn.
			currentPlayerColor = Field::Black;
			currentPlayerMove = makePlayerMove(board, playerBlack, currentPlayerColor);
			playerWhite->NotifyAboutOpponentMove(currentPlayerMove);

			drawGameBoard(board, FieldWidthInPixels, FieldHeightInPixels, fieldView, window, whiteBlockView, blackBlockView);

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

	SAFE_DELETE(playerWhite);
	SAFE_DELETE(playerBlack);

	// TODO: extract it to function.
	unsigned long long int allocationCounter = AllocationCounter::GetCounter();
	if (allocationCounter != 0uLL) {
		LOG_LN("Allocation counter is not zero. At least one 'new' has no corresponding 'delete'.");
		return false;
	}
	return true;
}

bool GameController::createPlayer(const int playerTypeId, Player*& player, const Board& board) {
	PlayerType playerType = static_cast<PlayerType>(playerTypeId);

	switch (playerType) {
		case PlayerType::HUMAN_CONSOLE:
			player = NEW(ConsolePlayer(&board));
			break;
		case PlayerType::BOT_RANDOMIZER:
			player = NEW(BotRandomizer(&board));
			break;
		case PlayerType::BOT_CM:
			player = NEW(CM::BotCM    (&board));
			break;
		case PlayerType::BOT_PB:
			player = NEW(PB::BotPB    (&board));
			break;
		default:
			LOG_ERROR("Wrong player value.");
			return false;
	}
	return true;
}

bool GameController::Initialize() {
	Random::Initialize();

	FileAppConfigLoader fileAppConfigLoader;
	bool result = fileAppConfigLoader.LoadFileAppConfigConstants(fileAppConfigContainer);
	if (!result) {
		return false;
	}
	return true;
}

Coordinates GameController::makePlayerMove(Board& board,
                                           Player* const player,
                                           const Field field) {
	Coordinates move = player->MakeMove();

	// Ensure that movement made by player is valid.
	bool result = false;
	bool isFieldEmpty = board.IsFieldEmpty(move.x, move.y, result);
	if (!result || !isFieldEmpty) {
		throw std::runtime_error("Unnamed error");
	}

	// Set player's movement on the board.
	result = board.SetField(move.x, move.y, field);
	if (!result) {
        throw std::runtime_error("Unnamed error");
	}
	return move;
}

void GameController::drawGameBoard(Board& board,
						  const std::size_t FieldWidthInPixels,
						  const std::size_t FieldHeightInPixels,
						  sf::RectangleShape& fieldView,
						  sf::RenderWindow& window,
						  sf::CircleShape& whiteBlockView,
						  sf::CircleShape& blackBlockView) {

	for (std::size_t y = 0u; y < board.getHeight(); ++y)
	{
		for (std::size_t x = 0u; x < board.getWidth(); ++x)
		{
			sf::Vector2f position(
				FieldWidthInPixels / 2 + x * FieldWidthInPixels,
				FieldHeightInPixels / 2 + y * FieldHeightInPixels
			);
			fieldView.setPosition(position);

			window.draw(fieldView);

			if (board.at(x, y) == Field::White)
			{
				whiteBlockView.setPosition(position);
				window.draw(whiteBlockView);
			}
			else if (board.at(x, y) == Field::Black)
			{
				blackBlockView.setPosition(position);
				window.draw(blackBlockView);
			}
		}
	}

	window.display();
}
