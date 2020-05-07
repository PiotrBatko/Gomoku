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

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

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

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
