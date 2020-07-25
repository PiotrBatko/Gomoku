#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <string>

#include "SFML/Graphics/Drawable.hpp"

class FpsCounter :
    public sf::Drawable
{
public:

    FpsCounter();
    void RegisterDrawnFrame();
    void RegisterElapsedSecond();
    void MakeVisible();
    void MakeInvisible();
    void ToggleVisibility();

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    int m_FramesInThisSecond = 0;
    std::string m_CachedLastFps = "-";
    bool m_Visible = false;
};

#endif
