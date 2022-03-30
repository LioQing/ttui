#include <ttui/core/mouse.hpp>

#include <cstdio>

namespace ttui { namespace mouse
{
    void EnableEvent(bool enable)
    {
        printf("%s\n", enable ? tcon::SetEnableMouseTracking().c_str() : tcon::SetDisableMouseTracking().c_str());
    }
}}