#include "GomokuView.hpp"

#include <chrono>

#include "SFML/Window.hpp"

#include "AppConfig/FileAppConfigContainer.hpp"
#include "DebugInfo.hpp"

GomokuView::~GomokuView()
{
    if (m_Thread.joinable())
    {
        LOG_ERROR("GomokuView thread wasn't joined before destruction");
        m_Thread.join();
    }
}

void GomokuView::Run()
{
    CreateWindow();
    MainLoop();
}

void GomokuView::ClearBoard()
{
    m_BoardView.Clear();
}

void GomokuView::RegisterMove(PawnColor color, Coordinates coordinates)
{
    m_BoardView.RegisterMove(color, coordinates);
}

void GomokuView::GameStarted(std::size_t numberOfColumns, std::size_t numberOfRows)
{
    m_BoardView.SetBoardDimensions(numberOfColumns, numberOfRows);
    m_WindowShouldBeOpened = true;

    m_Thread = std::thread(&GomokuView::Run, this);
}

void GomokuView::PawnAdded(PawnColor pawnColor, Coordinates coordinates)
{
    m_BoardView.RegisterMove(pawnColor, coordinates);
}

void GomokuView::GameFinished()
{
}

void GomokuView::Terminate()
{
    if (m_Thread.joinable())
    {
        m_WindowShouldBeOpened = false;
        m_Thread.join();
    }
}

void GomokuView::CreateWindow()
{
    m_Window.create(
        sf::VideoMode(
            BoardView::FieldWidthInPixels * m_BoardView.GetNumberOfColumns(),
            BoardView::FieldHeightInPixels * m_BoardView.GetNumberOfRows()
        ),
        "Gomoku Bot Battle"
    );
}

void GomokuView::MainLoop()
{
    using namespace std::chrono_literals;

    auto nextFpsUpdate = std::chrono::system_clock::now() + 1s;
    while (m_Window.isOpen())
    {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                GetGameModel().Terminate();
                return;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Key::F12)
                {
                    m_FpsCounter.ToggleVisibility();
                }
                break;

            default:
                break;
            }
        }

        if (std::chrono::system_clock::now() >= nextFpsUpdate)
        {
            m_FpsCounter.RegisterElapsedSecond();
            nextFpsUpdate += 1s;
        }

        Draw();

        if (not m_WindowShouldBeOpened)
        {
            m_Window.close();
        }
    }
}

void GomokuView::Draw()
{
    m_Window.clear(sf::Color::Black);
    m_Window.draw(m_BoardView);
    m_Window.draw(m_FpsCounter);
    m_Window.display();

    m_FpsCounter.RegisterDrawnFrame();
}
