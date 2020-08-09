#ifndef GOMOKUVIEW_HPP
#define GOMOKUVIEW_HPP

#include <chrono>
#include <thread>

#include "SFML/Graphics/RenderWindow.hpp"

#include "BoardView.hpp"
#include "Coordinates.hpp"
#include "FpsCounter.hpp"
#include "GameView.hpp"
#include "PawnColor.hpp"

class GomokuView :
    public GameView
{
public:

    void Run();

    void ClearBoard();
    void RegisterMove(PawnColor color, Coordinates coordinates);

private:

    void GameStarted(std::size_t numberOfColumns, std::size_t numberOfRows) override;
    void GameFinished() override;
    void Terminate() override;

    void CreateWindow();
    void MainLoop();
    void Draw();

    std::thread m_Thread;
    sf::RenderWindow m_Window;

    BoardView m_BoardView;
    FpsCounter m_FpsCounter;

    bool m_WindowShouldBeOpened = false;
};

#endif
