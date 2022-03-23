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

    bool Rect::IsOverlapped(const Rect& other) const
    {
        return 
            Top() < other.Bottom() &&
            Bottom() > other.Top() &&
            Left() < other.Right() &&
            Right() > other.Left();
    }

    bool Rect::TestPoint(uint16_t x, uint16_t y) const
    {
        return y >= Top() && y < Bottom() && x >= Left() && x < Right();
    }

    bool Rect::TestHorizontalLine(uint16_t y, uint16_t start_x, uint16_t end_x) const
    {
        return y >= Top() && y < Bottom() && start_x < Right() && end_x >= Left();
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