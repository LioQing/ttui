#include <ttui/core/Rect.hpp>

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

    bool Rect::operator==(const Rect& other) const
    {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    bool Rect::operator!=(const Rect& other) const
    {
        return !(operator==(other));
    }
}