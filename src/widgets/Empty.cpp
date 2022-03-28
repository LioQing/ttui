#include <ttui/widgets/Empty.hpp>

#include <ttui/core/Span.hpp>

namespace ttui
{
    Empty::Empty(const Border& border) : border(border) 
    {
    }

    Span Empty::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        return Span();
    }

    Border Empty::GetBorder() const
    {
        return border;
    }
}