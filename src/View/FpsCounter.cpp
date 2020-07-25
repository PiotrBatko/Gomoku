#include "FpsCounter.hpp"

#include "SFML/Graphics.hpp"

#include "FontsContainer.hpp"

FpsCounter::FpsCounter()
{
}

void FpsCounter::RegisterDrawnFrame()
{
    ++m_FramesInThisSecond;
}

void FpsCounter::RegisterElapsedSecond()
{
    m_CachedLastFps = std::to_string(m_FramesInThisSecond);
    m_FramesInThisSecond = 0;
}

void FpsCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    const sf::Font& font = fontsContainer.GetFont("default");
    sf::Text text = sf::Text(m_CachedLastFps, font, 20);
    text.setFillColor(sf::Color::Black);
    target.draw(text, states);
}
