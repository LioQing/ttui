#include <ttui/core/cursor.hpp>

#include <cstdio>

namespace ttui { namespace cursor
{
    void SetVisible(bool visible)
    {
        printf("%s\n", visible ? tcon::SetShowCursor().c_str() : tcon::SetHideCursor().c_str());
    }

    void SetPos(uint16_t x, uint16_t y)
    {
        printf("%s\n", tcon::SetCursorPos(x, y).c_str());
    }
}}