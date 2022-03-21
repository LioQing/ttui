#include <ttui/Rect.hpp>

namespace ttui
{
    Rect::Rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height) 
        : x(x), y(y), width(width), height(height)
    {
    }

    uint16_t Rect::Left() const
    {
        return x;
    }

    uint16_t Rect::Right() const
    {
        return x + width;
    }

    uint16_t Rect::Top() const
    {
        return y;
    }

    uint16_t Rect::Bottom() const
    {
        return y + height;
    }
}