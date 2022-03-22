#pragma once

#include <string>

#include <ttui/core/tcon.hpp>

namespace ttui
{
    struct Widget;

    struct Handle : tcon::Handle
    {
        std::string buf;

        void Render(const Widget& widget);

        void Draw();
    };
}