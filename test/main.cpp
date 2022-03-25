#include <thread>
#include <cmath>
#include <sys/ioctl.h>
#include <termios.h>
#include <clocale>

#include <ttui/ttui.hpp>

struct MyWidget : ttui::Widget
{
    std::string title;
    ttui::Rect rect;
    const ttui::Handle& handle;

    MyWidget(const ttui::Handle& handle) : handle(handle) {}

    ttui::Span GetSpan(uint16_t y, uint16_t& next_x, const ttui::Rect& rect) const override
    {
        if (y == 0 && next_x == 0)
        {
            next_x = 1;
            return ttui::Span();
        }
        else if (y == 0 && next_x == 1)
        {
            ttui::Appearance appear;
            appear.fg_color = ttui::Color::Green();
            appear.style = ttui::Style::Italic | ttui::Style::Blink;
            return ttui::Span(std::string("Width: ") + std::to_string(handle.GetWidth()), appear);
        }
        else if (y == 1 && next_x == 0)
        {
            next_x = 1;
            return ttui::Span();
        }
        else if (y == 1 && next_x == 1)
        {
            ttui::Appearance appear;
            appear.fg_color = ttui::Color::Green();
            appear.style = ttui::Style::Italic | ttui::Style::Blink;
            return ttui::Span(std::string("Height: ") + std::to_string(handle.GetHeight()), appear);
        }
        else if (y == 2 && next_x == 0)
        {
            next_x = 1;
            return ttui::Span();
        }
        else if (y == 2 && next_x == 1)
        {
            return ttui::Span(
                std::to_string(rect.x) + ' ' +
                std::to_string(rect.y) + ' ' +
                std::to_string(rect.width) + ' ' +
                std::to_string(rect.height));
        }
        else if (y < handle.GetHeight() - 3 && y > 0 && next_x == 0)
        {
            next_x = handle.GetWidth() / 2 - 7;
            return ttui::Span();
        }
        else if (y < handle.GetHeight() - 3 && y > 0 && next_x == handle.GetWidth() / 2 - 7)
        {
            return ttui::Span("Hello World!");
        }

        return ttui::Span();
    }

    ttui::Border GetBorder() const override
    {
        auto border = ttui::Border::Single(title);
        border.slices.at(ttui::Border::Right) = ttui::Border::Slice
        {
            "<",
            ttui::Appearance(ttui::Color::Red(), ttui::Color::BrightBlue(), ttui::Style::None)
        };
        return border;
    }
};

int main()
{
    // init
    ttui::Handle handle;
    handle.Init();
    printf("%s", (tcon::SetHideCursor() + tcon::SetClearScreen()).c_str());

    // variables
    bool is_running = true;
    ttui::Layout layout(ttui::Direction::Horizontal, ttui::Rect(1, 1, 100, 30));
    auto widget_y = 15;
    size_t cursor = 6;
    auto my_widget = MyWidget(handle);
    
    {
        my_widget.title = "My Widget";

        auto my_text = ttui::Text();

        my_text.paragraph.AddSpan(0, 0, ttui::Span("Hello World! This is first line."));

        my_text.paragraph.AddSpan(0, 50, ttui::Span("This is second appear." , 
            ttui::Appearance(ttui::Color::Red(), ttui::Color::Green(), ttui::Style::Italic)));

        my_text.paragraph.AddSpan(0, 72, ttui::Span("This is third", ttui::Appearance(ttui::Color::Blue())));

        my_text.paragraph.AddSpan(5, 4, ttui::Span("line 5 indented test"));
        my_text.paragraph.AddSpan(6, 0, ttui::Span("initial input value"));

        my_text.border = ttui::Border::Double("My Text");
        my_text.horiz_align = ttui::Align::Center;
        my_text.vert_align = ttui::Align::Center;

        layout.SetDefinitions(
            {
                ttui::Definition::Min(30),
                ttui::Definition::Relative(0.5),
                ttui::Definition::Min(30),
            }
        );

        ttui::Layout layout2(ttui::Direction::Vertical, ttui::Rect());
        layout2.SetDefinitions(
            {
                ttui::Definition::Max(20),
                ttui::Definition::Length(6),
            }
        );

        layout2.SetWidget(0, my_widget);
        layout2.SetWidget(1, my_widget);

        layout.SetWidget(0, my_text);
        layout.SetWidget(1, std::move(layout2));
        layout.SetWidget(2, my_widget);
    }

    auto& my_text = (ttui::Text&)layout.GetWidget(0);

    // main loop
    while (is_running)
    {
        // event polling
        ttui::Event event;
        while (handle.PollEvent(event))
        {
            if (event.type == ttui::Event::Exit) // exit event
            {
                is_running = false;
            }
            else if (event.type == ttui::Event::Resize)
            {
                printf("%s", tcon::SetClearScreen().c_str());
            }
            else if (event.type == ttui::Event::Input)
            {
                if (!event.input.is_esc)
                {
                    if (isprint(event.input.code) || event.input.code == '\n')
                    {
                        auto span = my_text.paragraph.GetSpan(cursor, 0);
                        span.str.push_back(event.input.code);
                        my_text.paragraph.SetSpan(cursor, 0, span);
                        if (event.input.code == '\n')
                            ++cursor;
                    }
                    else if (event.input.code == ttui::InputEvent::Backspace)
                    {
                        auto span = my_text.paragraph.GetSpan(cursor, 0);
                        if (!span.str.empty())
                        {
                            span.str.pop_back();
                            my_text.paragraph.SetSpan(cursor, 0, span);
                        }
                        else if (cursor > 6)
                        {
                            --cursor;
                        }
                    }
                }
                else
                {
                    if (event.input.code == ttui::InputEvent::Up)
                    {
                        ++widget_y;
                        printf("%s", tcon::SetClearScreen().c_str());
                    }
                    else if (event.input.code == ttui::InputEvent::Down)
                    {
                        --widget_y;
                        printf("%s", tcon::SetClearScreen().c_str());
                    }
                }
            }
        }

        // layout
        handle.Render(layout);
        handle.Draw();

        // roughly 60 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    // clean up
    printf("%s", (tcon::SetClearScreen() + tcon::SetShowCursor()).c_str());

    return 0;
}