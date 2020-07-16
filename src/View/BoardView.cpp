#include "BoardView.hpp"

#include "SFML/Graphics.hpp"

BoardView::BoardView() :
    m_WhitePawnView(PawnRadius, sf::Color(240, 240, 240)),
    m_BlackPawnView(PawnRadius, sf::Color(16, 16, 16)),
    m_FieldView(
        sf::Vector2f(
            FieldWidthInPixels,
            FieldHeightInPixels
        )
    )
{
}

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    DrawFields(target, states);
    DrawPawns(target, states);
}

void BoardView::DrawFields(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (std::size_t y = 0; y < m_NumberOfRows; ++y)
    {
        for (std::size_t x = 0; x < m_NumberOfColumns; ++x)
        {
            sf::Vector2f fieldCenter(
                FieldWidthInPixels / 2 + x * FieldWidthInPixels,
                FieldHeightInPixels / 2 + y * FieldHeightInPixels
            );

            m_FieldView.SetPosition(fieldCenter);
            target.draw(m_FieldView, states);
        }
    }
}

void BoardView::DrawPawns(sf::RenderTarget& target, sf::RenderStates states) const
{
}

std::size_t BoardView::GetNumberOfColumns() const
{
    return m_NumberOfColumns;
}

std::size_t BoardView::GetNumberOfRows() const
{
    return m_NumberOfRows;
}

void BoardView::SetBoardDimensions(std::size_t numberOfColumns, std::size_t numberOfRows)
{
    m_NumberOfColumns = numberOfColumns;
    m_NumberOfRows = numberOfRows;
}

void BoardView::Clear()
{
    m_Moves.clear();
}

void BoardView::RegisterMove(PawnColor color, Coordinates coordinates)
{
    m_Moves.emplace_back(color, coordinates);
}
