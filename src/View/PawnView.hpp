#ifndef PAWNVIEW_HPP
#define PAWNVIEW_HPP

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Drawable.hpp"

class PawnView :
    public sf::Drawable
{
public:

    PawnView(float radius, sf::Color color);
    void SetPosition(sf::Vector2f position);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape m_Shape;
};

#endif
