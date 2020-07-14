#ifndef BLOCKVIEW_HPP
#define BLOCKVIEW_HPP

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Drawable.hpp"

class BlockView :
    public sf::Drawable
{
public:

    BlockView(float radius, sf::Color color);
    void SetPosition(sf::Vector2f position);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::CircleShape m_Shape;
};

#endif
