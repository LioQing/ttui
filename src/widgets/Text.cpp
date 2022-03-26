#include <ttui/widgets/Text.hpp>

#include <ttui/core/Rect.hpp>

namespace ttui
{
    Span Text::GetSpan(uint16_t y, uint16_t& next_x, const Rect& rect) const
    {
        if (y == 0)
        {
            wrap_y_offset = 0;
            wrap_x_offset = 0;
        }

        // y align
        {
            int32_t tmp = rect.height - !border.is_none * 2 - paragraph.GetHeight();
            if (tmp > 0)
            {
                if (vert_align == Align::Center)
                    y -= tmp / 2;
                else if (vert_align == Align::Bottom)
                    y -= tmp;
            }
        }

        y -= wrap_y_offset;

        if (y < 0)
            return Span();

        if (wrap_start_offset > 0)
        {
            next_x = wrap_start_offset;
            wrap_start_offset = 0;
            return Span();
        }

        // line
        auto line_itr = paragraph.GetMap().find(y);
        if (line_itr == paragraph.GetMap().end() || line_itr->second.empty())
            return Span();

        // x align
        uint16_t widget_width = rect.width - !border.is_none * 2;
        uint16_t x_align = 0;
        {
            int32_t tmp = widget_width - paragraph.GetLineWidth(y);
            if (tmp > 0)
            {
                if (horiz_align == Align::Center)
                    x_align = tmp / 2;
                else if (horiz_align == Align::Right)
                    x_align = tmp;
            }
        }

        // span
        const auto& line = line_itr->second;
        
        auto span_itr = line.end();
        if (next_x + wrap_x_offset >= x_align)
        {
            span_itr = std::prev(line.upper_bound(next_x + wrap_x_offset - x_align));
        }
        
        if (span_itr == line.end())
        {
            if (next_x == 0)
                next_x = line.begin()->first + x_align;

            return Span();
        }

        Span span = span_itr->second;
        span.str = span.str.substr(wrap_x_offset > span_itr->first ? wrap_x_offset - span_itr->first : 0);

        auto next_itr = std::next(span_itr);

        if (is_wrapped && next_x + span.str.size() > (size_t)widget_width)
        {
            ++wrap_y_offset;
            wrap_x_offset += widget_width;
            wrap_x_offset -= wrap_x_offset % widget_width;
        }
        else if (next_itr == line.end())
        {
            wrap_x_offset = 0;
        }
        else if (is_wrapped && next_itr->first > widget_width)
        {
            wrap_y_offset += next_itr->first / widget_width;
            wrap_x_offset += next_itr->first;
            wrap_start_offset = next_itr->first % widget_width;
        }
        else
        {
            next_x = next_itr->first + x_align;
        }

        return span;
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}