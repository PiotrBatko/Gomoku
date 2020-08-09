#ifndef BOARDVIEW_HPP
#define BOARDVIEW_HPP

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Coordinates.hpp"
#include "FieldView.hpp"
#include "PawnColor.hpp"
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
    void DrawCoordinates(sf::RenderTarget& target, sf::RenderStates states) const;
    void DrawPawns(sf::RenderTarget& target, sf::RenderStates states) const;

    std::size_t GetNumberOfColumns() const;
    std::size_t GetNumberOfRows() const;
    void SetBoardDimensions(std::size_t numberOfColumns, std::size_t numberOfRows);

    void Clear();
    void RegisterMove(PawnColor color, Coordinates coordinates);

private:

    static constexpr float FieldPadding = 1.0F;
    static constexpr float PawnRadius = FieldWidthInPixels / 2 - 2 * FieldPadding;

    // These member variables are mutable, because during drawing (which must be
    // const function) they are moved, so their position is changing.
    mutable PawnView m_WhitePawnView;
    mutable PawnView m_BlackPawnView;
    mutable FieldView m_FieldView;
    mutable sf::Text m_CoordinatesView;

    std::size_t m_NumberOfColumns = 0;
    std::size_t m_NumberOfRows = 0;

    std::vector<std::pair<PawnColor, Coordinates>> m_Moves;
};

#endif
