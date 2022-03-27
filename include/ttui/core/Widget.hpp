#pragma once

#include <string>
#include <cstdint>
#include <functional>
#include <vector>

#include <ttui/core/Border.hpp>

namespace ttui
{
    struct Rect;
    struct Appearance;
    struct Span;
    struct Rect;
    struct Handle;

    struct Widget
    {
        virtual ~Widget() {}

        /**
         * @brief Get the span to be printed.
         * 
         * @param y The local y coordinate, always start at 0
         * @param x The local x coordinate, always start at 0 for each line
         * @param rect The bounding rectangle of the widget
         * @return Span The span
         */
        virtual Span GetSpan(uint16_t y, uint16_t x, const Rect& rect) const = 0;

        /**
         * @brief Get the border of the widget, default to Border::None().
         * 
         * @return Border The border
         */
        virtual Border GetBorder() const { return Border::None(); }

        /**
         * @brief This tells the handle to not render this widget, but still call Recursion(). Mostly for internal use.
         * 
         * @return bool Boolean indicating whether enabled or not
         */
        virtual bool Enabled() const { return true; }

        /**
         * @brief This will be called after the widget is rendered, tail call optimization is implemented. Mostly for internal use.
         * 
         * @param handle Reference to the handle
         * @param rect The bounding rectangle of the widget
         */
        virtual void Recursion(Handle& handle, const Rect& rect) const {}
    };
}