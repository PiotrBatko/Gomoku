#ifndef BOARDVIEW_HPP
#define BOARDVIEW_HPP

#include "SFML/Graphics/Drawable.hpp"

#include "FieldView.hpp"
#include "PawnView.hpp"

class BoardView :
    public sf::Drawable
{
public:

    static constexpr std::size_t FieldWidthInPixels = 25u;
    static constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;

    BoardView();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void DrawFields(sf::RenderTarget& target, sf::RenderStates states) const;
    void DrawPawns(sf::RenderTarget& target, sf::RenderStates states) const;

    std::size_t GetNumberOfColumns() const;
    std::size_t GetNumberOfRows() const;
    void SetBoardDimensions(std::size_t numberOfColumns, std::size_t numberOfRows);

private:

    static constexpr float FieldPadding = 1.0F;
    static constexpr float PawnRadius = FieldWidthInPixels / 2 - 2 * FieldPadding;

    mutable PawnView m_WhitePawnView;
    mutable PawnView m_BlackPawnView;
    mutable FieldView m_FieldView;

    std::size_t m_NumberOfColumns = 0;
    std::size_t m_NumberOfRows = 0;
};

#endif
