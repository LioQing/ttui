#pragma once

#include <ttui/tcon.hpp>

namespace ttui
{
    struct Widget;

    struct Handle : tcon::Handle
    {
        void Draw(const Widget& widget);
    };
}