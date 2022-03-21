#include <thread>
#include <cmath>
#include <sys/ioctl.h>
#include <termios.h>

#include <ttui/ttui.hpp>
#include <ttui/ttui.hpp>

struct MyWidget : ttui::Widget
{
    std::string GetString(uint16_t y, uint16_t& next_x, ttui::Appearance& appear) const override
    {
        if (y == 0 && next_x == 0) { next_x = 5; return "12345"; }
        else if (y == 0 && next_x == 5) { appear.SetForegroundColor(ttui::Color::Green()); return "5"; }
        else if (y == 1) { appear.SetBackgroundColor(ttui::Color::Red()); return "abcd"; }
        else if (y == 2) { appear = ttui::Appearance(); return "aaaaaaaaaaaaaaa"; }
        return "FAILED";
    }

    ttui::Rect GetRect() const override
    {
        return ttui::Rect(3, 3, 7, 3);
    }
};

int main()
{
    // init
    ttui::Handle handle;
    handle.Init();
    tcon::HideCursor();
    tcon::ClearScreen();

    // variables
    bool is_running = true;

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

        // draw
        tcon::SetStyle(ttui::Style::All, false);
        tcon::ResetColor(tcon::Target::Foreground);
        tcon::ResetColor(tcon::Target::Background);
        tcon::ClearScreen();

        // MyWidget
        handle.Draw(MyWidget());
        tcon::SetStyle(ttui::Style::All, false);
        tcon::ResetColor(tcon::Target::Foreground);
        tcon::ResetColor(tcon::Target::Background);

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // clean up
    tcon::ClearScreen();
    tcon::ShowCursor();

    return 0;
}