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

    std::string GetString(uint16_t y, uint16_t& next_x, ttui::Appearance& appear) const override
    {
        if      (y == 0 && next_x == 0) { next_x = 1; return ""; }
        else if (y == 0 && next_x == 1) { appear.fg_color = ttui::Color::Green(); appear.style = ttui::Style::Italic | ttui::Style::Blink; return std::to_string(handle.GetHeight()); }
        else if (y < handle.GetHeight() - 3 && y > 0 && next_x == 0) { next_x = handle.GetWidth() / 2 - 7; return ""; }
        else if (y < handle.GetHeight() - 3 && y > 0 && next_x == handle.GetWidth() / 2 - 7) { return "Hello World!"; }
        return "";
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

    // main loop
    while (is_running)
    {
        // event polling
        ttui::Event event;
        while (handle.PollEvent(event))
        {
            if (event.type == ttui::Event::Exit)        // exit event
            {
                is_running = false;
            }
        }

        // MyWidget
        handle.Render(my_widget);

        handle.Draw();

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // clean up
    printf("%s", (tcon::SetClearScreen() + tcon::SetShowCursor()).c_str());

    return 0;
}