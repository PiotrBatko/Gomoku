#include "GomokuView.hpp"

#include "SFML/Window.hpp"

#include "AppConfig/FileAppConfigContainer.hpp"

GomokuView::GomokuView() :
    m_FieldView(
        sf::Vector2f(
            FieldWidthInPixels,
            FieldHeightInPixels
        )
    )
{
}

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

        Draw();
    }
}

void GomokuView::Draw()
{
    m_Window.clear(sf::Color::Black);

    for (int y = 0; y < fileAppConfigContainer.BoardSize; ++y)
    {
        for (int x = 0; x < fileAppConfigContainer.BoardSize; ++x)
        {
            sf::Vector2f fieldCenter(
                FieldWidthInPixels / 2 + x * FieldWidthInPixels,
                FieldHeightInPixels / 2 + y * FieldHeightInPixels
            );

            m_FieldView.SetPosition(fieldCenter);
            m_Window.draw(m_FieldView);
        }
    }

    m_Window.display();
}
