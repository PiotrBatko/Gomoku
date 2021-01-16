#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <string>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Text.hpp"

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

    sf::Text m_Text;
    int m_FramesInThisSecond = 0;
    bool m_Visible = false;
};

#endif
