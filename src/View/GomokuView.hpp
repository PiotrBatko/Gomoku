#ifndef GOMOKUVIEW_HPP
#define GOMOKUVIEW_HPP

#include "SFML/Graphics/RenderWindow.hpp"

class GomokuView
{
public:

    void Run();

private:

    void CreateWindow();

    static constexpr std::size_t FieldWidthInPixels = 25u;
    static constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;

    sf::RenderWindow m_Window;
};

#endif
