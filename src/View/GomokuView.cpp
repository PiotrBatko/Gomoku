#include "GomokuView.hpp"

#include "SFML/Window.hpp"

#include "AppConfig/FileAppConfigContainer.hpp"

void GomokuView::Run()
{
    CreateWindow();
    MainLoop();
}

void GomokuView::CreateWindow()
{
    m_Window.create(
        sf::VideoMode(
            FieldWidthInPixels * fileAppConfigContainer.BoardSize,
            FieldHeightInPixels * fileAppConfigContainer.BoardSize
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
                return;

            default:
                break;
            }
        }
    }
}
