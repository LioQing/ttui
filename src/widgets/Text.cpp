#include <ttui/widgets/Text.hpp>

namespace ttui
{
    Span Text::GetSpan(uint16_t y, uint16_t& next_x) const
    {
        auto line_itr = paragraph.GetMap().find(y);
        if (line_itr == paragraph.GetMap().end() || line_itr->second.empty())
            return Span();

        const auto& line = line_itr->second;
        
        auto span_itr = line.find(next_x);
        if (span_itr == line.end())
        {
            if (next_x == 0)
                next_x = line.begin()->first;

            return Span();
        }

        auto next_itr = std::next(span_itr);
        if (next_itr != line.end())
            next_x = next_itr->first;
        return span_itr->second;
    }

    Rect Text::GetRect() const
    {
        return rect;
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}