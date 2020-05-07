#include <SFML/Graphics.hpp>

#include "Board.hpp"

int main()
{
    constexpr std::size_t BoardSize = 19;
    Board board(BoardSize, BoardSize);
    board.at(1, 0) = Field::White;
    board.at(1, 1) = Field::Black;
    board.at(2, 1) = Field::Black;
    board.at(3, 1) = Field::White;
    board.at(2, 2) = Field::White;

    constexpr std::size_t FieldWidthInPixels = 50;
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

        // Draw game board
        for (std::size_t y = 0; y < board.getHeight(); ++y)
        {
            for (std::size_t x = 0; x < board.getWidth(); ++x)
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

    return 0;
}
