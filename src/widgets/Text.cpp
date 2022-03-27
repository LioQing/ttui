#include <ttui/widgets/Text.hpp>

#include <ttui/core/Rect.hpp>

namespace ttui
{
    Span Text::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        if (!paragraph.HasSpan(y, x))
            return Span();
        
        return paragraph.GetSpan(y, x);
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}