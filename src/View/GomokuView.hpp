#ifndef GOMOKUVIEW_HPP
#define GOMOKUVIEW_HPP

#include "SFML/Graphics/RenderWindow.hpp"

#include "BoardView.hpp"
#include "Coordinates.hpp"
#include "PawnColor.hpp"

class GomokuView
{
public:

    void SetBoardDimensions(std::size_t width, std::size_t height);
    void Run();

    void ClearBoard();
    void RegisterMove(PawnColor color, Coordinates coordinates);

private:

    void CreateWindow();
    void MainLoop();
    void Draw();

    sf::RenderWindow m_Window;

    BoardView m_BoardView;
};

#endif
