#include <ttui/Appearance.hpp>

namespace ttui
{
    Appearance::Appearance(const Color& fg_color, const Color& bg_color, uint8_t style)
        : fg_color(fg_color), bg_color(bg_color), style(style)
    {
    }

    Appearance Appearance::Unchanged()
    {
        Appearance appear;
        appear.unchanged = true;
        return appear;
    }
}