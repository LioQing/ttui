#pragma once

#include <ttui/ttui.hpp>

namespace ttui
{
    /**
     * @brief A structure for specifying appearance of lines.
     * 
     */
    struct Appearance
    {
        /**
         * @brief Construct an Appearance object which resets all the appearances.
         * 
         */
        Appearance();

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
         * @brief Set the foreground color.
         * 
         * @param color The foreground color
         */
        void SetForegroundColor(const Color& color);

        /**
         * @brief Get the foreground color.
         * 
         * @return const Color& The foreground color
         */
        const Color& GetForegroundColor() const;

        /**
         * @brief Set the background color.
         * 
         * @param color The background color
         */
        void SetBackgroundColor(const Color& color);

        /**
         * @brief Get the background color.
         * 
         * @return const Color& The background color
         */
        const Color& GetBackgroundColor() const;

        /**
         * @brief Set the style.
         * 
         * @param style The style
         */
        void SetStyle(uint8_t style);

        /**
         * @brief Get the style.
         * 
         * @return uint8_t The style
         */
        uint8_t GetStyle() const;

    private:

        Color fg_color = Color::Reset();
        Color bg_color = Color::Reset();
        uint8_t style = 0;

        bool is_none;

        friend Handle;
    };
}