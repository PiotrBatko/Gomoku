#ifndef FIELDVIEW_HPP
#define FIELDVIEW_HPP

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class FieldView :
    public sf::Drawable
{
public:

    FieldView(sf::Vector2f dimensions);
    void SetPosition(sf::Vector2f position);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_Shape;
};

#endif
