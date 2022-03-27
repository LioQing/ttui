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

    /**
     * @brief A widget to display texts.
     * 
     */
    struct Text : Widget
    {
        Paragraph paragraph;
        Border border;
        Align vert_align = Align::Top;
        Align horiz_align = Align::Left;

        Text() = default;
        Text(const Text&) = default;

        Span GetSpan(uint16_t y, uint16_t x, const Rect& rect) const override;
        Border GetBorder() const override;
    };
}