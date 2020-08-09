#include "BoardView.hpp"

#include <sstream>

#include "SFML/Graphics.hpp"

#include "FontsContainer.hpp"

BoardView::BoardView() :
    m_WhitePawnView(PawnRadius, sf::Color(240, 240, 240)),
    m_BlackPawnView(PawnRadius, sf::Color(16, 16, 16)),
    m_FieldView(
        sf::Vector2f(
            FieldWidthInPixels,
            FieldHeightInPixels
        )
    ),
    m_CoordinatesView("-", fontsContainer.GetFont("default"), 10)
{
    m_CoordinatesView.setFillColor(sf::Color(234, 193, 128));
}

void BoardView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    DrawFields(target, states);
    DrawCoordinates(target, states);
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

void BoardView::DrawCoordinates(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (std::size_t y = 0; y < m_NumberOfRows; ++y)
    {
        for (std::size_t x = 0; x < m_NumberOfColumns; ++x)
        {
            std::ostringstream coordinates;
            coordinates << x << "\n" << y;

            sf::Vector2f fieldTopLeft(x * FieldWidthInPixels, y * FieldHeightInPixels);
            sf::Vector2f fieldCoordinatesMargin(3, 1);
            m_CoordinatesView.setPosition(fieldTopLeft + fieldCoordinatesMargin);
            m_CoordinatesView.setString(coordinates.str());

            target.draw(m_CoordinatesView, states);
        }
    }
}

void BoardView::DrawPawns(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& [color, coordinates] : m_Moves)
    {
        sf::Vector2f fieldCenter(
            FieldWidthInPixels / 2 + coordinates.x * FieldWidthInPixels,
            FieldHeightInPixels / 2 + coordinates.y * FieldHeightInPixels
        );

        if (color == PawnColor::White)
        {
            m_WhitePawnView.SetPosition(fieldCenter);
            target.draw(m_WhitePawnView, states);
        }
        else
        {
            m_BlackPawnView.SetPosition(fieldCenter);
            target.draw(m_BlackPawnView, states);
        }
    }
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
