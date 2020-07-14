#include "GomokuView.hpp"

#include "AppConfig/FileAppConfigContainer.hpp"

void GomokuView::Run()
{
    CreateWindow();
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
