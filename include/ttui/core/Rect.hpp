#pragma once

#include <cstdint>

namespace ttui
{
    struct Rect
    {
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;

        Rect() = default;
        Rect(const Rect& rect) = default;

        Rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

        uint16_t Left() const;
        uint16_t Right() const;
        uint16_t Top() const;
        uint16_t Bottom() const;

        bool IsOverlapped(const Rect& other) const;
        bool TestPoint(uint16_t x, uint16_t y) const;
        bool TestHorizontalLine(uint16_t y, uint16_t start_x, uint16_t end_x) const;

        bool operator==(const Rect& other) const;
        bool operator!=(const Rect& other) const;
    };
}