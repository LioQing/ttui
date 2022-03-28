#pragma once

#include <ttui/core/Widget.hpp>
#include <ttui/core/Border.hpp>

namespace ttui
{
    struct Span;

    struct Empty : Widget
    {
        Border border = Border::None();

        Empty() = default;
        Empty(const Empty&) = default;
        Empty(const Border& border);

        Span GetSpan(uint16_t y, uint16_t x, const Rect& rect) const override;
        Border GetBorder() const override;
    };
}