#include "PawnView.hpp"

#include "SFML/Graphics.hpp"

PawnView::PawnView(float radius, sf::Color color) :
    m_Shape(radius)
{
    m_Shape.setFillColor(color);
    m_Shape.setOrigin(radius, radius);
}

void PawnView::SetPosition(sf::Vector2f position)
{
    m_Shape.setPosition(position);
}

void PawnView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Shape, states);
}
