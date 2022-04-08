#pragma once

#include <map>
#include <string>
#include <deque>
#include <functional>

#include <ttui/core/tcon.hpp>
#include <ttui/core/Rect.hpp>

namespace ttui
{
    struct Widget;

    /**
     * @brief A structure to handle the terminal, performing retrieval of information, event polling, rendering, drawing.
     * 
     */
    struct Handle : tcon::Handle
    {
        std::string buf;

        /**
         * @brief Initialize tcon in this Handle.
         * 
         * @return bool Boolean indicating success or not
         */
        bool Init();

        /**
         * @brief Render a widget given its bounding rectangle.
         * 
         * @param widget The widget, has to be of derived from Widget
         * @param rect The bounding rectangle
         */
        void Render(const Widget& widget, const Rect& rect);

        /**
         * @brief Render a widget with the terminal size.
         * 
         * @param widget The widget, has to be of derived from Widget
         */
        void Render(const Widget& widget);

        /**
         * @brief Draw the rendered widgets.
         * 
         * @param predraw_seq The escape sequence to be printed before drawing
         * @param postdraw_seq The escape sequence to be printed after drawing
         */
        void Draw(const std::string& predraw_seq = "", const std::string& postdraw_seq = "");

    private:

        void HiddenRender(const Widget& widget, const Rect& rect);
        
        std::multimap<uint16_t, Rect> drawn_rects;
        std::deque<std::function<void()>> pps;
        bool in_render = false;
    };
}