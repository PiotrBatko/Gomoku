#include "FpsCounter.hpp"

#include "SFML/Graphics.hpp"

#include "FontsContainer.hpp"

FpsCounter::FpsCounter() :
    m_Text("-", fontsContainer.GetFont("default"), 20)
{
    m_Text.setFillColor(sf::Color::Black);
}

void FpsCounter::RegisterDrawnFrame()
{
    ++m_FramesInThisSecond;
}

void FpsCounter::RegisterElapsedSecond()
{
    m_Text.setString(std::to_string(m_FramesInThisSecond));
    m_FramesInThisSecond = 0;
}

void FpsCounter::MakeVisible()
{
    m_Visible = true;
}

void FpsCounter::MakeInvisible()
{
    m_Visible = false;
}

void FpsCounter::ToggleVisibility()
{
    m_Visible = not m_Visible;
}

void FpsCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (not m_Visible)
    {
        return;
    }

    target.draw(m_Text, states);
}
