#include "FieldView.hpp"

#include "SFML/Graphics.hpp"

FieldView::FieldView(sf::Vector2f dimensions) :
    m_Shape(dimensions)
{
    m_Shape.setFillColor(sf::Color(176, 144, 90));
    m_Shape.setOutlineThickness(-0.5F);
    m_Shape.setOutlineColor(sf::Color(214, 177, 114));
    m_Shape.setOrigin(dimensions.x / 2, dimensions.y / 2);
}

void FieldView::SetPosition(sf::Vector2f position)
{
    m_Shape.setPosition(position);
}

void FieldView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Shape, states);
}
