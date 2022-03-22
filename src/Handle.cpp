#include <ttui/Handle.hpp>

#include <cstdint>
#include <ttui/Widget.hpp>
#include <ttui/Rect.hpp>
#include <ttui/Appearance.hpp>

namespace
{
    std::string ProcessAppearance(const ttui::Appearance& appear)
    {
        if (appear.unchanged)
            return "";

        auto ProcessColor = [](const ttui::Color& color, bool is_foreground) -> std::string
        {
            auto target = is_foreground ? tcon::Target::Foreground : tcon::Target::Background;

            if      ((int)color.type < (int)ttui::Color::Type::Reset) return tcon::Color4bit((int)color.type, target);
            else if (color.type == ttui::Color::Type::Reset)          return tcon::ColorReset(target);
            else if (color.type == ttui::Color::Type::RGB)            return tcon::Color24bit(color.rgb, target);
            else if (color.type == ttui::Color::Type::Color8bit)      return tcon::Color8bit(color.color_8bit, target);
            else return "";
        };

        return tcon::SetAppearance(
            ProcessColor(appear.fg_color, true) +
            ProcessColor(appear.bg_color, false) +
            tcon::Style(tcon::Style::All, false) +
            tcon::Style(appear.style, true)
        );
    }
}

namespace ttui
{
    void Handle::Draw(const Widget& widget)
    {
        auto rect = widget.GetRect();
        auto border = widget.GetBorder();
        std::string buf;

        for (uint16_t y = 0; y < rect.height; ++y)
        {
            // draw border
            {
                auto* left = &border.slices.at(Border::Left);
                auto* right = &border.slices.at(Border::Right);

                uint8_t mid_slice;
                std::string mid_str;

                if (y == 0)
                {
                    left = &border.slices.at(Border::TopLeft);
                    right = &border.slices.at(Border::TopRight);

                    mid_slice = Border::Top;
                    for (uint16_t i = 0; i < rect.width - 2; ++i)
                        mid_str += border.slices.at(mid_slice).str;
                }
                else if (y == rect.height - 1)
                {
                    left = &border.slices.at(Border::BottomLeft);
                    right = &border.slices.at(Border::BottomRight);

                    mid_slice = Border::Bottom;
                    for (uint16_t i = 0; i < rect.width - 2; ++i)
                        mid_str += border.slices.at(mid_slice).str;
                }

                buf += 
                    tcon::SetCursorPos(rect.x, rect.y + y) +
                    ProcessAppearance(left->appear) +
                    left->str;

                if (!mid_str.empty())
                {
                    buf +=
                        ProcessAppearance(border.slices.at(Border::Bottom).appear) +
                        mid_str;
                }
                else
                {
                    buf += tcon::SetCursorPos(rect.x + rect.width - 1, rect.y + y);
                }

                buf +=
                    ProcessAppearance(right->appear) +
                    right->str;
            }

            // draw string
            for (uint16_t x = 0, next_x = 0; x < rect.width; x = next_x)
            {
                Appearance appear;
                auto str = widget.GetString(y, next_x, appear);

                if (x + str.size() > rect.width)
                {
                    str.resize(rect.width - x);
                }

                buf +=
                    tcon::SetCursorPos(rect.x + x, rect.y + y) +
                    ProcessAppearance(appear) +
                    str;

                if (next_x == x)
                {
                    break;
                }
            }
        }

        printf("%s", buf.c_str());
        fflush(stdout);
    }
}