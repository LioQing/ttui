#pragma once

#include <string>
#include <cstdint>

#include <ttui/core/Border.hpp>

namespace ttui
{
    struct Rect;
    struct Appearance;
    struct Span;

    struct Widget
    {
        virtual ~Widget() {}

        /**
         * @brief Get the span to be printed.
         * 
         * @param y The local y coordinate, always start at 0
         * @param next_x Passed in as current x coordinate, always start at 0, expect to get next x coordinate (if unchanged, next call will be new line, y incremented)
         * @return Span The span
         */
        virtual Span GetSpan(uint16_t y, uint16_t& next_x) const = 0;

        /**
         * @brief Get the rectangle of the widget.
         * 
         * @return Rect The rectangle
         */
        virtual Rect GetRect() const = 0;

        /**
         * @brief Get the border of the widget, default to Border::None().
         * 
         * @return Border The border
         */
        virtual Border GetBorder() const { return Border::None(); }
    };
}