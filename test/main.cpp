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
        return ttui::Rect(10, 15, 30, 15);
    }

    ttui::Border GetBorder() const override
    {
        auto border = ttui::Border::Single();
        // border.slices.at(ttui::Border::Right) = ttui::Border::Slice
        // {
        //     "<",
        //     ttui::Appearance(ttui::Color::Red(), ttui::Color::BrightBlue(), ttui::Style::None)
        // };
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

    my_text.paragraph.AddSpan(0, 0, ttui::Span("Hello World! This is first line."));

    my_text.paragraph.AddSpan(0, 50, ttui::Span("This is second appear." , 
        ttui::Appearance(ttui::Color::Red(), ttui::Color::Green(), ttui::Style::Italic)));

    my_text.paragraph.AddSpan(0, 72, ttui::Span("This is third", ttui::Appearance(ttui::Color::Blue())));

    my_text.paragraph.AddSpan(5, 4, ttui::Span("line 5 indented test"));
    my_text.paragraph.AddSpan(6, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(7, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(8, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(9, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(10, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(11, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(12, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(13, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(14, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(15, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(16, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(17, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(18, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(19, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(20, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(21, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(22, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(23, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(24, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(25, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(26, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(27, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(28, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(29, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(30, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(31, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(32, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(33, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    my_text.paragraph.AddSpan(34, 0, ttui::Span("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

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
            else if (event.type == ttui::Event::Resize)
            {
                printf("%s", tcon::SetClearScreen().c_str());
                fflush(stdout);
            }
        }

        my_text.rect = ttui::Rect(1, 3, handle.GetWidth() - 20, handle.GetHeight() - 10);

        // MyWidget
        handle.Render(my_widget);
        handle.Render(my_text);
        handle.Draw();

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // clean up
    printf("%s", (tcon::SetClearScreen() + tcon::SetShowCursor()).c_str());

    return 0;
}