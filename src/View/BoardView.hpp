#ifndef BOARDVIEW_HPP
#define BOARDVIEW_HPP

#include "SFML/Graphics/Drawable.hpp"

#include "BlockView.hpp"
#include "FieldView.hpp"

class BoardView :
    public sf::Drawable
{
public:

    static constexpr std::size_t FieldWidthInPixels = 25u;
    static constexpr std::size_t FieldHeightInPixels = FieldWidthInPixels;

    BoardView();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void DrawFields(sf::RenderTarget& target, sf::RenderStates states) const;
    void DrawBlocks(sf::RenderTarget& target, sf::RenderStates states) const;

    std::size_t GetNumberOfColumns() const;
    std::size_t GetNumberOfRows() const;
    void SetBoardDimensions(std::size_t numberOfColumns, std::size_t numberOfRows);

private:

    static constexpr float FieldPadding = 1.0F;
    static constexpr float BlockRadius = FieldWidthInPixels / 2 - 2 * FieldPadding;

    mutable BlockView m_WhiteBlockView;
    mutable BlockView m_BlackBlockView;
    mutable FieldView m_FieldView;

    std::size_t m_NumberOfColumns = 0;
    std::size_t m_NumberOfRows = 0;
};

#endif
