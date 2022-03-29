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

    ttui::cursor::SetVisible(false);

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
        ttui::Text my_text_widget;
        my_text_widget.SetBorder(ttui::Border::Single("Typing Test"));
        my_text_widget.SetHorizAlign(ttui::Align::Center);
        my_text_widget.SetWrap(ttui::Wrap::Word);

        ttui::Paragraph paragraph;

        paragraph.AddSpan(0, 0, ttui::Span("Type something:"));
        paragraph.AddSpan(1, 0, ttui::Span("", ttui::Appearance(ttui::Color::BrightYellow())));

        my_text_widget.SetParagraph(paragraph);

        layout.SetWidget(0, my_text_widget);

        // inner layout
        ttui::Layout inner_layout;
        inner_layout.SetDirection(ttui::Direction::Vertical);
        inner_layout.SetDefinitions({
            ttui::Definition::Absolute(10),
            ttui::Definition::Fit(),
        });

        inner_layout.SetWidget(0, MyWidget());

        // list widget
        ttui::List list;
        list.SetBorder(ttui::Border::Single("List (up/down keys)"));
        list.SetWrap(ttui::Wrap::Word);
        list.SetEnableAutoAppearance(true);

        list.SetItems(
        {
            ttui::Paragraph("Item 1", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Red())),
            ttui::Paragraph("Item 2", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Yellow())),
            ttui::Paragraph("Item 3", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Green())),
            ttui::Paragraph("Item 4 - Long Long Item Name Hey This is Very Long Right Now", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Blue())),
            ttui::Paragraph("Item 5", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Magenta())),
            ttui::Paragraph("Item 6 - Long Space in Between         Item", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Cyan())),
            ttui::Paragraph("Item 7", ttui::Appearance(ttui::Color::Reset(), ttui::Color::Black())),
        });

        inner_layout.SetWidget(1, list);

        layout.SetWidget(1, std::move(inner_layout));
    }

    ttui::Text& my_text = (ttui::Text&)layout.GetWidget(0);
    ttui::List& my_list = (ttui::List&)((ttui::Layout&)layout.GetWidget(1)).GetWidget(1);

    auto typing_cursor = 1;

    // main loop
    bool is_running = true;
    handle.Render(layout);
    handle.Draw();
    while (is_running)
    {
        // poll events
        ttui::Event event;
        bool has_event = false;
        while (handle.PollEvent(event))
        {
            if (event.type == ttui::Event::Exit)
            {
                is_running = false;
                break;
            }
            else if (event.type == ttui::Event::Resize)
            {
                ttui::screen::Clear();
            }
            else if (event.type == ttui::Event::Input) // typing
            {
                if (!event.input.is_esc)
                {
                    if (isprint(event.input.code) || event.input.code == '\n')
                    {
                        auto my_text_para = my_text.GetParagraph();
                        auto span = my_text_para.GetSpan(typing_cursor, 0);

                        span.str.push_back(event.input.code);
                        my_text_para.SetSpan(typing_cursor, 0, span);
                        if (event.input.code == '\n')
                            ++typing_cursor;
                        
                        my_text.SetParagraph(my_text_para);
                    }
                    else if (event.input.code == ttui::InputEvent::Backspace)
                    {
                        auto my_text_para = my_text.GetParagraph();
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

                        my_text.SetParagraph(my_text_para);
                    }
                }
                else if (event.input.code == ttui::InputEvent::Up) // list navigation
                {
                    auto idx = my_list.GetSelectedItemIndex();
                    if (idx != 0)
                        my_list.SetSelectedItemIndex(idx - 1);
                }
                else if (event.input.code == ttui::InputEvent::Down) // list navigation
                {
                    auto idx = my_list.GetSelectedItemIndex();
                    if (idx != my_list.GetSize() - 1)
                        my_list.SetSelectedItemIndex(idx + 1);
                }
            }

            has_event = true;
        }

        // render
        if (has_event)
        {
            handle.Render(layout);
            handle.Draw();
        }

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    // clean up
    ttui::cursor::SetVisible(true);
    ttui::screen::Clear();

    return 0;
}