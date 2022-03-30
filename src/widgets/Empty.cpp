#include <ttui/widgets/Empty.hpp>

#include <ttui/core/Span.hpp>
#include <ttui/core/Rect.hpp>

namespace ttui
{
    Empty::Empty(const Border& border, const Color& color)
        : border(border), color(color)
    {
    }

    Span Empty::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        return Span(std::string(rect.width, ' '), ttui::Appearance(ttui::Color::Reset(), color));
    }

    Border Empty::GetBorder() const
    {
        return border;
    }
}