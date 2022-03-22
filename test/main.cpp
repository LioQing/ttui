#include <thread>
#include <cmath>
#include <sys/ioctl.h>
#include <termios.h>
#include <clocale>

#include <ttui/ttui.hpp>

struct MyWidget : ttui::Widget
{
    const ttui::Handle& handle;

    MyWidget(const ttui::Handle& handle) : handle(handle) {}

    ttui::Span GetSpan(uint16_t y, uint16_t& next_x) const override
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

    ttui::Rect GetRect() const override
    {
        return ttui::Rect(0, 1, handle.GetWidth(), handle.GetHeight());
    }

    ttui::Border GetBorder() const override
    {
        auto border = ttui::Border::Single();
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
    auto my_widget = MyWidget(handle);
    auto my_text = ttui::Text();
    my_text.paragraph.SetMap(
        {
            { 0, 
                { 
                    { 0, ttui::Span("Hello World! This is first line.") },
                    { 32, ttui::Span("This is second appear." , ttui::Appearance(ttui::Color::Red(), ttui::Color::Green(), ttui::Style::Italic)) },
                    { 54, ttui::Span("This is third", ttui::Appearance(ttui::Color::Blue())) },
                }
            },
            { 5,
                {
                    { 4, ttui::Span("line 5 indented test") },
                }
            }
        }
    );
    my_text.border = ttui::Border::Double();

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
        }

        my_text.rect = ttui::Rect(0, 1, handle.GetWidth(), handle.GetHeight());

        // MyWidget
        handle.Render(my_text);

        handle.Draw();

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // clean up
    printf("%s", (tcon::SetClearScreen() + tcon::SetShowCursor()).c_str());

    return 0;
}