#include <ttui/widgets/Text.hpp>

#include <ttui/core/Wrap.hpp>

namespace ttui
{
    Text::Text(const Paragraph& paragraph, Border border, Wrap wrap)
        : paragraph(paragraph), border(border), wrap(wrap)
    {
    }

    void Text::SetParagraph(const Paragraph& paragraph)
    {
        this->paragraph = paragraph;
        updated = true;
    }

    const Paragraph& Text::GetParagraph() const
    {
        return paragraph;
    }

    void Text::SetBorder(const Border& border)
    {
        this->border = border;
    }

    void Text::SetHorizAlign(Align align)
    {
        horiz_align = align;
        updated = true;
    }

    Align Text::GetHorizAlign() const
    {
        return horiz_align;
    }

    void Text::SetVertAlign(Align align)
    {
        vert_align = align;
        updated = true;
    }

    Align Text::GetVertAlign() const
    {
        return vert_align;
    }

    void Text::SetWrap(Wrap wrap)
    {
        this->wrap = wrap;
        updated = true;
    }

    Wrap Text::GetWrap() const
    {
        return wrap;
    }

    void Text::SetBackgroundColor(const Color& color)
    {
        bg_color = color;
    }

    const Color& Text::GetBackgroundColor() const
    {
        return bg_color;
    }

    void Text::SetScrollHeight(uint16_t height)
    {
        scroll_height = height;
    }

    uint16_t Text::GetScrollHeight() const
    {
        return scroll_height;
    }

    const Paragraph& Text::GetWrappedParagraph(uint16_t width) const
    {
        GetSpan(0, 0, Rect(0, 0, width, 1));
        return wrapped_para;
    }

    Span Text::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        // scroll
        y += scroll_height;

        // wrap
        const Paragraph* para = &paragraph;
        if (wrap != Wrap::None)
        {
            if (updated || rect != prev_rect)
            {
                wrapped_para = paragraph.Wrapped(wrap, rect.width);
                prev_rect = rect;
                updated = false;
            }
            para = &wrapped_para;
        }

        // y align
        int32_t actual_y = y;
    
        auto line_height = para->GetHeight();
        if (rect.height > line_height)
        {
            if (vert_align == Align::Center)
                actual_y -= (rect.height - line_height) / 2;
            else if (vert_align == Align::Bottom)
                actual_y -= rect.height - line_height;
        }
        
        if (actual_y < 0)
            return Span(" ", ttui::Appearance(ttui::Color::Reset(), bg_color));

        if (!para->HasLine(actual_y))
            return Span(" ", ttui::Appearance(ttui::Color::Reset(), bg_color));

        // x align
        int32_t actual_x = x;

        auto line_width = para->GetLineWidth(actual_y);
        if (rect.width > line_width)
        {
            if (horiz_align == Align::Center)
                actual_x -= (rect.width - line_width) / 2;
            else if (horiz_align == Align::Right)
                actual_x -= rect.width - line_width;
        }
        
        if (actual_x < 0)
            return Span(" ", ttui::Appearance(ttui::Color::Reset(), bg_color));

        if (!para->HasSpan(actual_y, actual_x))
            return Span(" ", ttui::Appearance(ttui::Color::Reset(), bg_color));
        
        return para->GetSpan(actual_y, actual_x);
    }

    Border Text::GetBorder() const
    {
        return border;
    }
}