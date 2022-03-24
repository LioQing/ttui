#include <ttui/widgets/Text.hpp>

#include <ttui/core/Rect.hpp>

namespace ttui
{
    Span Text::GetSpan(uint16_t y, uint16_t& next_x, const Rect& rect) const
    {
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

        // line
        auto line_itr = paragraph.GetMap().find(y);
        if (line_itr == paragraph.GetMap().end() || line_itr->second.empty())
            return Span();

        // align
        uint16_t x_align = 0;
        {
            int32_t tmp = rect.width - !border.is_none * 2 - paragraph.GetLineWidth(y);
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
        
        auto span_itr = next_x >= x_align ? line.find(next_x - x_align) : line.end();
        if (span_itr == line.end())
        {
            if (next_x == 0)
                next_x = line.begin()->first + x_align;

            return Span();
        }

        auto next_itr = std::next(span_itr);
        if (next_itr != line.end())
            next_x = next_itr->first + x_align;
        return span_itr->second;
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}