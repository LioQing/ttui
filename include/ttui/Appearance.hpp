#pragma once

#include <ttui/Color.hpp>

namespace ttui
{
    /**
     * @brief A structure for specifying appearance of lines.
     * 
     */
    struct Appearance
    {
        /**
         * @brief Construct an Appearance object which resets all the appearance fields.
         * 
         */
        Appearance() = default;

        Appearance(const Appearance&) = default;

        /**
         * @brief Construct an Appearance object.
         * 
         * @param fg_color The foreground color
         * @param bg_color The background color
         * @param style The style
         */
        Appearance(const Color& fg_color, const Color& bg_color, uint8_t style);

        /**
         * @brief Construct a special Appearance object which preserve the previous appearance used, all other fields are ignored.
         * 
         * @return Appearance The special unchanged version of Appearance class
         */
        static Appearance Unchanged();

        Color fg_color = Color::Reset();
        Color bg_color = Color::Reset();
        uint8_t style = 0;

        bool unchanged = false;
    };
}