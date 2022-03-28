#include <ttui/core/screen.hpp>

#include <cstdio>
#include <ttui/core/tcon.hpp>

namespace ttui { namespace screen
{
    void Clear(const Color& color)
    {
        std::string color_code;

        if      ((int)color.type < (int)ttui::Color::Type::Reset) color_code = tcon::SetAppearance({ tcon::Color4bit((int)color.type, tcon::Target::Background) });
        else if (color.type == ttui::Color::Type::Reset)          color_code = tcon::SetAppearance({ tcon::ColorReset(tcon::Target::Background) });
        else if (color.type == ttui::Color::Type::RGB)            color_code = tcon::SetAppearance({ tcon::Color24bit(color.rgb, tcon::Target::Background) });
        else if (color.type == ttui::Color::Type::Color8bit)      color_code = tcon::SetAppearance({ tcon::Color8bit(color.color_8bit, tcon::Target::Background) });

        printf("%s\n", (color_code + tcon::SetClearScreen()).c_str());
    }
}}