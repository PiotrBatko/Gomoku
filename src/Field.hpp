#ifndef FIELD_HPP
#define FIELD_HPP

#include "PawnColor.hpp"

class Field
{
public:

    enum Value
    {
        White = static_cast<int>(PawnColor::White),
        Black = static_cast<int>(PawnColor::Black),
        Empty,
    };

    constexpr Field() = default;
    constexpr Field(Value value) :
        m_Value(value)
    {
    }

    constexpr Field(PawnColor color) :
        m_Value(color == PawnColor::White ? White : Black)
    {
    }

    constexpr bool operator==(const Field rhs) const
    {
        return m_Value == rhs.m_Value;
    }

    constexpr bool operator!=(const Field rhs) const
    {
        return not (*this == rhs);
    }

private:

    Value m_Value = Empty;
};

#endif
