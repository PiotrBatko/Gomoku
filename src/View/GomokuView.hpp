#ifndef GOMOKUVIEW_HPP
#define GOMOKUVIEW_HPP

#include "SFML/Graphics/RenderWindow.hpp"

#include "FieldView.hpp"

class GomokuView
{
public:

    GomokuView();
    void Run();

private:

    void CreateWindow();
    void MainLoop();
    void Draw();

    static constexpr std::size_t FieldWidthInPixels = 25u;
    static constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;

    sf::RenderWindow m_Window;
    FieldView m_FieldView;
};

#endif
