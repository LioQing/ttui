#include <ttui/core/cursor.hpp>

#include <cstdio>

namespace ttui { namespace cursor
{
    void SetVisible(bool visible)
    {
        printf("%s", (visible ? tcon::SetShowCursor() : tcon::SetHideCursor()).c_str());
        fflush(stdout);
    }

    void SetPos(uint16_t x, uint16_t y)
    {
        printf("%s", tcon::SetCursorPos(x, y).c_str());
        fflush(stdout);
    }
}}