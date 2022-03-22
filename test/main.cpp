#include <thread>
#include <cmath>
#include <sys/ioctl.h>
#include <termios.h>
#include <clocale>

#include <ttui/ttui.hpp>

struct MyWidget : ttui::Widget
{
    std::string GetString(uint16_t y, uint16_t& next_x, ttui::Appearance& appear) const override
    {
        if      (y == 0 && next_x == 0) { next_x = 1; return ""; }
        else if (y == 0 && next_x == 1) { return " MyWidget "; }
        else if (y == 2 && next_x == 0) { next_x = 2; return ""; }
        else if (y == 2 && next_x == 2) { return "Hello World!"; }
        else return "";
    }

    ttui::Rect GetRect() const override
    {
        return ttui::Rect(3, 3, 16, 5);
    }

    ttui::Border GetBorder() const override
    {
        return ttui::Border::Single();
    }
};

int main()
{
    setlocale(LC_ALL, "C.UTF-8");

    // init
    ttui::Handle handle;
    handle.Init();
    printf("%s", (tcon::SetHideCursor() + tcon::SetClearScreen()).c_str());

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
        printf("%s", (
            tcon::SetAppearance(
                tcon::Style(ttui::Style::All, false),
                tcon::ColorReset(tcon::Target::Foreground),
                tcon::ColorReset(tcon::Target::Background)
            ) +
            tcon::SetClearScreen()
        ).c_str());
        
        // MyWidget
        handle.Draw(MyWidget());
        printf("%s", 
            tcon::SetAppearance(
                tcon::Style(ttui::Style::All, false),
                tcon::ColorReset(tcon::Target::Foreground),
                tcon::ColorReset(tcon::Target::Background)
            ).c_str()
        );

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // clean up
    printf("%s", (tcon::SetClearScreen() + tcon::SetShowCursor()).c_str());

    return 0;
}