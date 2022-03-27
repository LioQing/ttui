#include <ttui/widgets/Text.hpp>

#include <ttui/core/Rect.hpp>

namespace ttui
{
    Span Text::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        // y align
        int32_t actual_y = y;
        {
            auto tmp = rect.height - paragraph.GetHeight();
            if (tmp > 0)
            {
                if (vert_align == Align::Center)
                    actual_y -= tmp / 2;
                else if (vert_align == Align::Bottom)
                    actual_y -= tmp;
            }
        }
        
        if (actual_y < 0)
            return Span();

        if (!paragraph.HasLine(actual_y))
            return Span();

        // x align
        int32_t actual_x = x;
        {
            auto tmp = rect.width - paragraph.GetLineWidth(actual_y);
            if (tmp > 0)
            {
                if (horiz_align == Align::Center)
                    actual_x -= tmp / 2;
                else if (horiz_align == Align::Right)
                    actual_x -= tmp;
            }
        }
        
        if (actual_x < 0)
            return Span();

        if (paragraph.HasSpan(actual_y, actual_x))
            return paragraph.GetSpan(actual_y, actual_x);
        
        return Span();
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}