#include <ttui/Handle.hpp>

#include <cstdint>
#include <ttui/Widget.hpp>
#include <ttui/Rect.hpp>
#include <ttui/Appearance.hpp>

namespace
{
    void ProcessAppearance(const ttui::Appearance& appear)
    {
        auto ProcessColor = [](const ttui::Color& color, bool is_foreground)
        {
            auto target = is_foreground ? tcon::Target::Foreground : tcon::Target::Background;

            if      ((int)color.type < (int)ttui::Color::Type::Reset) tcon::SetColor4bit((int)color.type, target);
            else if (color.type == ttui::Color::Type::Reset)          tcon::ResetColor(target);
            else if (color.type == ttui::Color::Type::RGB)            tcon::SetColor24bit(color.rgb, target);
            else if (color.type == ttui::Color::Type::Color8bit)      tcon::SetColor8bit(color.color_8bit, target);
        };

        ProcessColor(appear.GetForegroundColor(), true);
        ProcessColor(appear.GetBackgroundColor(), false);
        tcon::SetStyle(tcon::Style::All, false);
        tcon::SetStyle(appear.GetStyle(), true);
    }
}

namespace ttui
{
    void Handle::Draw(const Widget& widget)
    {
        auto rect = widget.GetRect();

        for (uint16_t y = 0; y < rect.height; ++y)
        {
            for (uint16_t x = 0, next_x = 0; x < rect.width; x = next_x)
            {
                tcon::SetCursorPos(rect.x + x, rect.y + y);

                Appearance appear;
                appear.is_none = true;
                auto line = widget.GetString(y, next_x, appear);

                if (!appear.is_none)
                {
                    ProcessAppearance(appear);
                }

                if (x + line.size() > rect.width)
                {
                    line.resize(rect.width - x);
                }

                printf("%s", line.c_str());

                if (next_x == x)
                {
                    break;
                }
            }
        }

        fflush(stdout);
    }
}