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
        {               
            ProcessColor(appear.fg_color, true),
            ProcessColor(appear.bg_color, false),
            tcon::Style(~appear.style, false),
            tcon::Style(appear.style, true)
        });
    }
}

namespace ttui
{
    void Handle::Render(const Widget& widget)
    {
        auto rect = widget.GetRect();
        auto border = widget.GetBorder();

        for (uint16_t y = 0; y < rect.height; ++y)
        {
            // draw border
            std::string right_border_str;
            if (!border.is_none)
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

                auto temp = ProcessAppearance(right->appear);

                buf +=
                    tcon::SetCursorPos(rect.x, rect.y + y) +
                    ProcessAppearance(left->appear) +
                    left->str;

                if (!mid_str.empty())
                {
                    buf +=
                        ProcessAppearance(border.slices.at(Border::Bottom).appear) +
                        mid_str + 
                        tcon::SetCursorPos(rect.x + rect.width, rect.y + y) +
                        temp +
                        right->str;

                    continue;
                }
                else
                {
                    right_border_str = 
                        tcon::SetCursorPos(rect.x + rect.width, rect.y + y) +
                        temp +
                        right->str;
                }
            }

            // draw string
            uint16_t offset = 0;
            if (!border.is_none)
                offset = 1;
            uint16_t widget_width = rect.width - offset * 2;

            for (uint16_t x = 0, next_x = 0; x < widget_width; x = next_x)
            {
                Appearance appear;
                auto str = widget.GetString(y - offset, next_x, appear);
                std::string padding;

                if (x + str.size() > widget_width)
                {
                    str.resize(widget_width - x);
                }
                else if (str.size() < (size_t)next_x - x)
                {
                    padding.resize(next_x - x - str.size(), ' ');
                }
                else if (next_x == x)
                {
                    padding.resize(widget_width - x - str.size(), ' ');
                    next_x = widget_width;
                }

                buf +=
                    ProcessAppearance(appear) +
                    str + 
                    tcon::SetAppearance({ tcon::ColorReset(tcon::Target::Background) }) +
                    padding;
            }

            buf += right_border_str;
        }
    }

    void Handle::Draw()
    {
        printf("%s", buf.c_str());
        fflush(stdout);
        buf.clear();
    }
}