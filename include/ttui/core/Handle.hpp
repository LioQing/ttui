#pragma once

#include <map>
#include <string>

#include <ttui/core/tcon.hpp>
#include <ttui/core/Rect.hpp>

namespace ttui
{
    struct Widget;

    struct Handle : tcon::Handle
    {
        std::string buf;

        void Render(const Widget& widget);

        void Draw();

    private:
        
        std::map<uint16_t, Rect> drawn_rects;
    };
}