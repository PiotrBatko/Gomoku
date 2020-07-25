#include "GomokuView.hpp"

#include "SFML/Window.hpp"

#include "AppConfig/FileAppConfigContainer.hpp"
#include "FontsContainer.hpp"

GomokuView::GomokuView()
{
    fontsContainer.LoadFont("default", "res/fonts/Ubuntu-L.ttf");
}

void GomokuView::SetBoardDimensions(std::size_t numberOfColumns, std::size_t numberOfRows)
{
    m_BoardView.SetBoardDimensions(numberOfColumns, numberOfRows);
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
    while (m_Window.isOpen())
    {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                m_Window.close();
                return;

            default:
                break;
            }
        }

        Draw();
    }
}

void GomokuView::Draw()
{
    m_Window.clear(sf::Color::Black);
    m_Window.draw(m_BoardView);
    m_Window.display();
}
