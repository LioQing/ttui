#pragma once

#include <string>
#include <cstdint>

#include <ttui/Border.hpp>

namespace ttui
{
    struct Rect;
    struct Appearance;

    struct Widget
    {
        virtual ~Widget() {}

        /**
         * @brief Get the string to be printed.
         * 
         * @param y The local y coordinate
         * @param next_x Next string's local x coordinate (if unchanged or exceed width of widget, y will be incremented to a new line)
         * @param appear The appearance of the string (if unchanged, previous appearance will be preserved)
         * @return std::string The string to be printed
         */
        virtual std::string GetString(uint16_t y, uint16_t& next_x, Appearance& appear) const = 0;

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