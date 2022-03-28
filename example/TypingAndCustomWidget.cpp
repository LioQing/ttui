#include <iostream>
#include <thread>
#include <ttui/ttui.hpp>

// custom class as widget
struct MyWidget : ttui::Widget
{
    std::string title = "My Widget Title!";

    ttui::Span GetSpan(uint16_t y, uint16_t x, const ttui::Rect& rect) const override
    {
        if (y == 0 && x == 1)
        {
            return ttui::Span("This is my custom widget!", 
                ttui::Appearance(ttui::Color::Red(), ttui::Color::Reset(), ttui::Style::Blink));
        }
        else if (y == 1 && x == 1)
        {
            return ttui::Span("Press Ctrl + C to exit", 
                ttui::Appearance(ttui::Color::Red(), ttui::Color::Reset(), ttui::Style::Blink));
        }

        return ttui::Span();
    }

    ttui::Border GetBorder() const override
    {
        auto border = ttui::Border::Double(title, ttui::Appearance(ttui::Color::Green()));
        return border;
    }
};

int main()
{
    // init
    ttui::Handle handle;
    handle.Init();

    std::cout << 
        tcon::SetHideCursor() <<
        tcon::SetClearScreen();
    std::cout.flush();

    // layout and widgets
    ttui::Layout layout;
    {
        // create the main layout
        layout.SetDirection(ttui::Direction::Horizontal);
        layout.SetDefinitions({
            ttui::Definition::Relative(0.7f),
            ttui::Definition::Fit(),
        });

        // text widget
        ttui::Text my_text;
        my_text.border = ttui::Border::Single("Typing Test");
        my_text.horiz_align = ttui::Align::Center;
        my_text.wrap = ttui::Wrap::Word;

        my_text.paragraph.AddSpan(0, 0, ttui::Span("Type something:"));
        my_text.paragraph.AddSpan(1, 0, ttui::Span("", ttui::Appearance(ttui::Color::BrightYellow())));

        layout.SetWidget(0, my_text);

        // inner layout
        ttui::Layout inner_layout;
        inner_layout.SetDirection(ttui::Direction::Vertical);
        inner_layout.SetDefinitions({
            ttui::Definition::Absolute(10),
            ttui::Definition::Fit(),
        });

        inner_layout.SetWidget(0, MyWidget());

        ttui::Text empty;
        empty.border = ttui::Border::Single();
        inner_layout.SetWidget(1, empty);

        layout.SetWidget(1, std::move(inner_layout));
    }

    ttui::Paragraph& my_text_para = ((ttui::Text&)layout.GetWidget(0)).paragraph;
    auto typing_cursor = 1;

    // main loop
    bool is_running = true;
    while (is_running)
    {
        // poll events
        ttui::Event event;
        while (handle.PollEvent(event))
        {
            if (event.type == ttui::Event::Exit)
            {
                is_running = false;
                break;
            }
            else if (event.type == ttui::Event::Resize)
            {
                std::cout << tcon::SetClearScreen() << std::endl;
            }
            else if (event.type == ttui::Event::Input)
            {
                if (!event.input.is_esc)
                {
                    if (isprint(event.input.code) || event.input.code == '\n')
                    {
                        auto span = my_text_para.GetSpan(typing_cursor, 0);
                        span.str.push_back(event.input.code);
                        my_text_para.SetSpan(typing_cursor, 0, span);
                        if (event.input.code == '\n')
                            ++typing_cursor;
                    }
                    else if (event.input.code == ttui::InputEvent::Backspace)
                    {
                        auto span = my_text_para.GetSpan(typing_cursor, 0);
                        if (!span.str.empty())
                        {
                            span.str.pop_back();
                            my_text_para.SetSpan(typing_cursor, 0, span);
                        }
                        else if (typing_cursor > 1)
                        {
                            --typing_cursor;
                        }
                    }
                }
            }
        }

        // render
        handle.Render(layout);

        handle.Draw();

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    // clean up
    std::cout << 
        tcon::SetShowCursor() <<
        tcon::SetClearScreen();
    std::cout.flush();

    return 0;
}