#include <ttui/widgets/Text.hpp>

#include <ttui/core/Wrap.hpp>

namespace ttui
{
    Span Text::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        // wrap
        const Paragraph* para = &paragraph;
        if (wrap != Wrap::None)
        {
            if (wrap != prev_wrap || rect != prev_rect)
            {
                wrapped_para = paragraph.Wrapped(wrap, rect.width);
                prev_wrap = wrap;
                prev_rect = rect;
            }
            para = &wrapped_para;
        }

        // y align
        int32_t actual_y = y;
    
        auto line_height = para->GetHeight();
        if (rect.height > line_height)
        {
            if (vert_align == Align::Center)
                actual_y -= (rect.height - line_height) / 2;
            else if (vert_align == Align::Bottom)
                actual_y -= rect.height - line_height;
        }
        
        if (actual_y < 0)
            return Span();

        if (!para->HasLine(actual_y))
            return Span();

        // x align
        int32_t actual_x = x;

        auto line_width = para->GetLineWidth(actual_y);
        if (rect.width > line_width)
        {
            if (horiz_align == Align::Center)
                actual_x -= (rect.width - line_width) / 2;
            else if (horiz_align == Align::Right)
                actual_x -= rect.width - line_width;
        }
        
        if (actual_x < 0)
            return Span();

        if (!para->HasSpan(actual_y, actual_x))
            return Span();
        
        return para->GetSpan(actual_y, actual_x);
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}