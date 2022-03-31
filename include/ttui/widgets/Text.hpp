#pragma once

#include <vector>
#include <map>

#include <ttui/core/Widget.hpp>
#include <ttui/core/Paragraph.hpp>
#include <ttui/core/Border.hpp>
#include <ttui/core/Align.hpp>
#include <ttui/core/Rect.hpp>

namespace ttui
{
    struct Rect;

    /**
     * @brief A widget to display texts.
     * 
     */
    struct Text : Widget
    {
        Text() = default;
        Text(const Text&) = default;

        Text(const Paragraph& paragraph, Border border = Border::Single(), Wrap wrap = Wrap::None);

        void SetParagraph(const Paragraph& paragraph);
        const Paragraph& GetParagraph() const;

        void SetBorder(const Border& border);

        void SetHorizAlign(Align align);
        Align GetHorizAlign() const;

        void SetVertAlign(Align align);
        Align GetVertAlign() const;

        void SetWrap(Wrap wrap);
        Wrap GetWrap() const;

        void SetBackgroundColor(const Color& color);
        const Color& GetBackgroundColor() const;

        void SetScrollHeight(uint16_t height);
        uint16_t GetScrollHeight() const;
        
        Span GetSpan(uint16_t y, uint16_t x, const Rect& rect) const override;
        Border GetBorder() const override;
    
    private:

        Paragraph paragraph;
        Border border = Border::Single();
        Align vert_align = Align::Top;
        Align horiz_align = Align::Left;
        Wrap wrap = Wrap::None;
        Color bg_color = Color::Reset();

        uint16_t scroll_height = 0;

        mutable bool updated = true;
        mutable Rect prev_rect = Rect();
        mutable Paragraph wrapped_para;
    };
}