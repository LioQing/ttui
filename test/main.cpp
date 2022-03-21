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
        if (y == 0) return "1234567";
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

    char hello_world[] = "Hello World!";
    int32_t col[] = { 0xff0000, 0xff0000, 0xff7f00, 0xff7f00, 0xffff00, 0xffff00, 0x00ff00, 0x00ff00, 0x0000ff, 0x0000ff, 0x9400d3, 0x9400d3 };
    float len = 0.f;

    char circle[] = "circle";
    float theta = 0.f;
    int rot_dir = 1;
    int radius = 10;

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
            else if (event.type == ttui::Event::Input)  // input event
            {
                if (!event.input.is_esc && !event.input.is_alt && event.input.code == ' ') // space pressed
                {
                    rot_dir = -rot_dir;
                }
            }
        }

        // update
        len = fmod(len + .33f, 16);
        theta = fmod(theta + rot_dir * .05f, 2 * 3.1415926);

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

        // hello world
        tcon::SetCursorPos(handle.GetWidth() / 2 - 6, handle.GetHeight() / 2);
        tcon::SetStyle(ttui::Style::Italic | ttui::Style::Underline, true);
        for (int i = 0; i < 12; ++i)
        {
            if (i < len)
            {
                tcon::SetColor24bit(col[i], tcon::Target::Foreground);
                printf("%c", hello_world[i]);
            }
            else
            {
                printf(" ");
            }
        }

        // circle
        tcon::SetCursorPos(handle.GetWidth() / 2 - 2 + (int)(2 * radius * cos(theta)), handle.GetHeight() / 2 + (int)(radius * sin(theta)));
        tcon::SetStyle(ttui::Style::All, false);
        tcon::SetColor4bit(tcon::Color::BrightBlue, tcon::Target::Foreground);
        tcon::ResetColor(tcon::Target::Background);
        printf("%s", circle);

        fflush(stdout);

        // roughly 30 fps
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    // clean up
    tcon::ClearScreen();
    tcon::ShowCursor();

    return 0;
}