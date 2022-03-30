#pragma once

#include <ttui/core/Widget.hpp>
#include <ttui/core/Border.hpp>
#include <ttui/core/Color.hpp>

namespace ttui
{
    struct Span;

    struct Empty : Widget
    {
        Border border = Border::None();
        Color color = Color::Reset();

        Empty() = default;
        Empty(const Empty&) = default;
        Empty(const Border& border, const Color& color = Color::Reset());

        Span GetSpan(uint16_t y, uint16_t x, const Rect& rect) const override;
        Border GetBorder() const override;
    };
}