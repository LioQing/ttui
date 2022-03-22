#pragma once

#include <cstdint>

namespace ttui
{
    struct Style
    {
        enum : uint8_t
        {
            None            = 0,
            Bold            = 0x1,
            Dim             = 0x2,
            Italic          = 0x4,
            Underline       = 0x8,
            Blink           = 0x10,
            Inversed        = 0x20,
            Invisible       = 0x40,
            CrossedOut      = 0x80,

            All = Bold | Dim | Italic | Underline | Blink | 
                Inversed | Invisible | CrossedOut,
        };
    };
}