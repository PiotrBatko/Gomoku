#include "BlockView.hpp"

#include "SFML/Graphics.hpp"

BlockView::BlockView(float radius, sf::Color color) :
    m_Shape(radius)
{
    m_Shape.setFillColor(color);
    m_Shape.setOrigin(radius, radius);
}

void BlockView::SetPosition(sf::Vector2f position)
{
    m_Shape.setPosition(position);
}

void BlockView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Shape, states);
}
