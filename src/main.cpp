#include <SFML/Graphics.hpp>
#include <iostream>

#include "Board.hpp"
#include "Random.h"
#include "CommonUtils.h"

#include "BotRandomizer.hpp"
#include "HumanPlayer.hpp"

int main()
{
	Random::Initialize();

    constexpr std::size_t BoardSize = 19u;
    Board board(BoardSize, BoardSize);

    #if 0 // Code for testing.
    board.SetField(1, 0, Field::White);
    board.SetField(1, 1, Field::Black);
    board.SetField(2, 1, Field::Black);
    board.SetField(3, 1, Field::White);
    board.SetField(2, 2, Field::White);
	#endif

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

    Player* playerWhite = NEW(HumanPlayer  (&board));
	Player* playerBlack = NEW(BotRandomizer(&board));

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

        bool battleFinished = false;
        while (!battleFinished) {

			///*
        	std::size_t x, y;
        	// 1. White player's turn.
        	bool result = playerWhite->MakeMove(x, y);
			if (!result) {
				return false;
			}

			result = false;
			bool isFieldEmpty = board.IsFieldEmpty(x, y, result);
			if (!result || !isFieldEmpty) {
				return 1;
			}
			result = board.SetField(x, y, Field::White);
			if (!result) {
				return 1;
			}

			// 2. Black player's turn.
			result = playerBlack->MakeMove(x, y);
			if (!result) {
				return false;
			}

			result = false;
			isFieldEmpty = board.IsFieldEmpty(x, y, result);
			if (!result || !isFieldEmpty) {
				return 1;
			}
			result = board.SetField(x, y, Field::Black);
			if (!result) {
				return 1;
			}
			//*/

			// Draw game board
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
    }

    return 0;
}
