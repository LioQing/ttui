#include <ttui/Appearance.hpp>

namespace ttui
{
    Appearance::Appearance() : is_none(false)
    {
    }

    Appearance::Appearance(const Color& fg_color, const Color& bg_color, uint8_t style)
        : fg_color(fg_color), bg_color(bg_color), style(style), is_none(false)
    {
    }

    void Appearance::SetForegroundColor(const Color& color)
    {
        fg_color = color;
        is_none = false;
    }

    const Color& Appearance::GetForegroundColor() const
    {
        return fg_color;
    }

    void Appearance::SetBackgroundColor(const Color& color)
    {
        bg_color = color;
        is_none = false;
    }

    const Color& Appearance::GetBackgroundColor() const
    {
        return bg_color;
    }

    void Appearance::SetStyle(uint8_t style)
    {
        style = style;
        is_none = false;
    }

    uint8_t Appearance::GetStyle() const
    {
        return style;
    }
}