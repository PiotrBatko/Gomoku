#ifndef GOMOKUVIEW_HPP
#define GOMOKUVIEW_HPP

#include <chrono>

#include "SFML/Graphics/RenderWindow.hpp"

#include "BoardView.hpp"
#include "Coordinates.hpp"
#include "FpsCounter.hpp"
#include "PawnColor.hpp"

class GomokuView
{
public:

    void SetBoardDimensions(std::size_t numberOfColumns, std::size_t numberOfRows);
    void Run();

    void ClearBoard();
    void RegisterMove(PawnColor color, Coordinates coordinates);

private:

    void CreateWindow();
    void MainLoop();
    void Draw();

    sf::RenderWindow m_Window;

    BoardView m_BoardView;
    FpsCounter m_FpsCounter;
};

#endif
