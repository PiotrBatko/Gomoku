#ifndef GOMOKUVIEW_HPP
#define GOMOKUVIEW_HPP

#include "SFML/Graphics/RenderWindow.hpp"

#include "BoardView.hpp"

class GomokuView
{
public:

    void SetBoardDimensions(std::size_t width, std::size_t height);
    void Run();

private:

    void CreateWindow();
    void MainLoop();
    void Draw();

    sf::RenderWindow m_Window;

    BoardView m_BoardView;
};

#endif
