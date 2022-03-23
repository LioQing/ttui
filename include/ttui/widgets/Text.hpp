#pragma once

#include <vector>
#include <map>

#include <ttui/core/Widget.hpp>
#include <ttui/core/Paragraph.hpp>
#include <ttui/core/Rect.hpp>
#include <ttui/core/Border.hpp>

namespace ttui
{
    struct Text : Widget
    {
        Paragraph paragraph;
        Rect rect;
        Border border;

        Text() = default;
        Text(const Text&) = default;

        Span GetSpan(uint16_t y, uint16_t& next_x) const override;
        Rect GetRect() const override;
        Border GetBorder() const override;
    };
}