#pragma once

#include <vector>
#include <map>

#include <ttui/core/Widget.hpp>
#include <ttui/core/Paragraph.hpp>
#include <ttui/core/Border.hpp>
#include <ttui/core/Align.hpp>

namespace ttui
{
    struct Rect;

    struct Text : Widget
    {
        Paragraph paragraph;
        Border border;
        Align vert_align = Align::Top;
        Align horiz_align = Align::Left;
        bool is_wrapped = false;

        Text() = default;
        Text(const Text&) = default;

        Span GetSpan(uint16_t y, uint16_t& next_x, const Rect& rect) const override;
        Border GetBorder() const override;

    private:

        mutable uint16_t wrap_y_offset = 0;
        mutable size_t wrap_x_offset = 0;
        mutable uint16_t wrap_start_offset = 0;
    };
}