#include <ttui/core/mouse.hpp>

#include <cstdio>

namespace ttui { namespace mouse
{
    void EnableEvent(bool enable)
    {
        printf("%s", (enable ? tcon::SetEnableMouseTracking() : tcon::SetDisableMouseTracking()).c_str());
        fflush(stdout);
    }
}}