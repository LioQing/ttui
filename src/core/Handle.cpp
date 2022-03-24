#include <ttui/core/Handle.hpp>

#include <cstdint>
#include <algorithm>

#include <ttui/core/Widget.hpp>
#include <ttui/core/Appearance.hpp>
#include <ttui/core/Span.hpp>

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

    bool IsInIntervals(uint16_t x, const std::map<uint16_t, uint16_t>& intervals)
    {
        for (const auto& i : intervals)
        {
            if (x >= i.first && x < i.second)
                return true;
        }
        return false;
    }

    std::map<uint16_t, uint16_t> ProjectInterval(uint16_t start, uint16_t end, const std::map<uint16_t, uint16_t>& intervals)
    {
        if (intervals.empty())
            return std::map<uint16_t, uint16_t>{ { start, end } };

        std::map<uint16_t, uint16_t> projected;
        auto last = start;
        
        for (const auto& i : intervals)
        {
            if (i.first > end)
                break;
            
            if (i.second < last)
                continue;
            
            if (last < i.first)
                projected.emplace(last, i.first);
            
            last = i.second;
        }

        if (last < end)
            projected.emplace(last, end);

        return projected;
    }
}

namespace ttui
{
    void Handle::Render(const Widget& widget, const Rect& rect)
    {
        if (widget.Enabled())
            HiddenRender(widget, rect);

        pps.emplace_back([this, &widget, rect](){ widget.Recursion(*this, rect); });

        if (in_render)
            return;
        
        in_render = true;
        
        while (!pps.empty())
        {
            pps.front()();
            pps.pop_front();
        }

        in_render = false;
    }

    void Handle::Render(const Widget& widget)
    {
        Render(widget, Rect(1, 1, GetWidth(), GetHeight()));
    }

    void Handle::Draw()
    {
        printf("%s", buf.c_str());
        fflush(stdout);

        buf.clear();
        drawn_rects.clear();
    }

    void Handle::HiddenRender(const Widget& widget, const Rect& rect)
    {
        if (rect.width == 0 || rect.height == 0)
            return;

        const auto border = widget.GetBorder();

        uint16_t offset = !border.is_none;
        uint16_t widget_width = rect.width < offset * 2 ? 0 : rect.width - offset * 2;

        for (uint16_t y = 0; y < rect.height; ++y)
        {
            // check already drawn rects
            std::map<uint16_t, uint16_t> drawn_intervals;
            for (const auto& r : drawn_rects)
            {
                if (!r.second.TestHorizontalLine(rect.y + y, rect.Left(), rect.Right()))
                    continue;
                
                if (drawn_intervals.empty() || drawn_intervals.rbegin()->second < r.first)
                    drawn_intervals.emplace(r.first, r.second.Right());
                else
                    drawn_intervals.rbegin()->second = r.second.Right();
            }

            // draw border
            std::string right_border_str;
            if (!border.is_none)
            {
                auto* left = &border.slices.at(Border::Left);
                auto* right = &border.slices.at(Border::Right);

                uint8_t mid_slice = 0;

                if (y == 0)
                {
                    left = &border.slices.at(Border::TopLeft);
                    right = &border.slices.at(Border::TopRight);

                    mid_slice = Border::Top;
                }
                else if (y == rect.height - 1)
                {
                    left = &border.slices.at(Border::BottomLeft);
                    right = &border.slices.at(Border::BottomRight);

                    mid_slice = Border::Bottom;
                }

                if (!IsInIntervals(rect.x, drawn_intervals))
                {
                    buf +=
                        tcon::SetCursorPos(rect.x , rect.y + y) +
                        ProcessAppearance(left->appear) +
                        left->str;
                }

                if (mid_slice != 0)
                {
                    buf += ProcessAppearance(border.slices.at(mid_slice).appear);
                    for (const auto& i : ProjectInterval(rect.x + 1, rect.Right() - 1, drawn_intervals))
                    {
                        buf += tcon::SetCursorPos(i.first, rect.y + y);
                        for (uint16_t j = 0; j < (int32_t)i.second - i.first; ++j)
                        {
                            if (mid_slice == Border::Top && i.first + j >= rect.x + 1 && i.first + j < rect.x + border.title.size() + 1)
                            {
                                buf += border.title.at(j);
                            }
                            else
                            {
                                buf += border.slices.at(mid_slice).str;
                            }
                        }
                    }

                    if (!IsInIntervals(rect.Right() - 1, drawn_intervals))
                    {
                        buf +=
                            tcon::SetCursorPos(rect.Right() - 1, rect.y + y) +
                            ProcessAppearance(right->appear) +
                            right->str;
                    }

                    continue;
                }
                else if (!IsInIntervals(rect.Right() - 1, drawn_intervals))
                {
                    right_border_str = 
                        tcon::SetCursorPos(rect.Right() - 1, rect.y + y) +
                        ProcessAppearance(right->appear) +
                        right->str;
                }
            }

            // draw string
            for (uint16_t x = 0, next_x = 0; x < widget_width; x = next_x)
            {
                Span span = widget.GetSpan(y - offset, next_x, rect);

                if (x + span.str.size() > widget_width)
                {
                    span.str.resize(widget_width - x);
                }
                
                if (next_x == x || next_x > widget_width)
                {
                    next_x = widget_width;
                }

                if (!span.str.empty())
                {
                    buf += ProcessAppearance(span.appear);
                    for (const auto& i : ProjectInterval(rect.x + offset + x, rect.x + offset + x + span.str.size(), drawn_intervals))
                    {
                        buf += 
                            tcon::SetCursorPos(i.first, rect.y + y) +
                            span.str.substr(i.first - x - rect.x - offset, i.second > i.first ? i.second - i.first : 0);
                    }
                }

                if ((size_t)rect.x + offset + next_x > rect.x + offset + x + span.str.size())
                {
                    buf += tcon::SetAppearance({ tcon::ColorReset(tcon::Target::Background) });
                    for (const auto& i : ProjectInterval(rect.x + offset + x + span.str.size(), rect.x + offset + next_x, drawn_intervals))
                    {
                        buf += 
                            tcon::SetCursorPos(i.first, rect.y + y) +
                            std::string(i.second > i.first ? i.second - i.first : 0, ' ');
                    }
                }
            }

            buf += right_border_str;
        }
        
        drawn_rects.emplace(rect.Left(), rect);
    }
}