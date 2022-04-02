#include <ttui/widgets/List.hpp>

#include <algorithm>
#include <stdexcept>

namespace ttui
{
    List::List(const std::vector<Paragraph>& items, Border border, Wrap wrap)
        : items(items), border(border), wrap(wrap)
    {
    }

    void List::SetItems(const std::vector<Paragraph>& items)
    {
        this->items = items;
        updated = true;
    }

    const std::vector<Paragraph>& List::GetItems() const
    {
        return items;
    }

    void List::SetItemAt(uint16_t index, const Paragraph& item)
    {
        items.at(index) = item;
        updated = true;
    }

    const Paragraph& List::GetItemAt(uint16_t index) const
    {
        return items.at(index);
    }

    void List::PushBack(const Paragraph& item)
    {
        items.push_back(item);
        updated = true;
    }

    size_t List::GetSize() const
    {
        return items.size();
    }

    void List::SetBorder(const Border& border)
    {
        this->border = border;
    }

    void List::SetHorizAlign(Align align)
    {
        horiz_align = align;
        updated = true;
    }

    Align List::GetHorizAlign() const
    {
        return horiz_align;
    }

    void List::SetWrap(Wrap wrap)
    {
        this->wrap = wrap;
        updated = true;
    }

    Wrap List::GetWrap() const
    {
        return wrap;
    }

    void List::SetScrollHeight(uint16_t height)
    {
        scroll_height = height;
        selected_item_updated = true;
    }

    uint16_t List::GetScrollHeight() const
    {
        return scroll_height;
    }

    void List::SetFillSpanBackgroundColor(bool enabled)
    {
        fill_span_bg_color = enabled;
    }

    bool List::IsFillSpanBackgroundColor() const
    {
        return fill_span_bg_color;
    }

    void List::SetSelectedItemIndex(size_t item_index)
    {
        if (item_index >= items.size())
            throw std::out_of_range("item_index");
        
        selected_item_idx = item_index;
        selected_item_updated = true;
    }

    size_t List::GetSelectedItemIndex() const
    {
        return selected_item_idx;
    }

    void List::SetEnableAutoAppearance(bool enabled)
    {
        enable_auto_appear = enabled;
    }

    bool List::IsAutoAppearanceEnabled() const
    {
        return enable_auto_appear;
    }

    void List::SetAutoAppearance(const Appearance& unselected, const Appearance& selected)
    {
        auto_appear_unselected = unselected;
        auto_appear_selected = selected;
    }

    const Appearance& List::GetAutoAppearance(bool selected) const
    {
        if (selected)
            return auto_appear_selected;
        else
            return auto_appear_unselected;
    }

    const std::vector<Paragraph>& List::GetWrappedItems(uint16_t width) const
    {
        GetSpan(0, 0, Rect(0, 0, width, 1));
        return wrapped_items;
    }

    Span List::GetSpan(uint16_t y, uint16_t x, const Rect& rect) const
    {
        // wrap and update y position
        const std::vector<Paragraph>* items_ptr = &items;
        if (wrap != Wrap::None)
        {
            if (updated || prev_rect != rect)
            {
                // wrap all items
                wrapped_items.clear();
                y_item_idx.clear();
                for (const Paragraph& item : items)
                {
                    wrapped_items.push_back(item.Wrapped(wrap, rect.width));

                    // set y position of each wrapped item
                    if (wrapped_items.size() == 1)
                        y_item_idx.push_back(0);
                    else
                        y_item_idx.push_back(y_item_idx.back() + (wrapped_items.rbegin() + 1)->GetHeight());
                }

                // update prevs
                prev_rect = rect;
                updated = false;
                selected_item_updated = true;
            }

            items_ptr = &wrapped_items;
        }
        else if (updated || prev_rect != rect)
        {
            // set y position of each item
            y_item_idx.clear();
            for (uint16_t i = 0; i < items.size(); ++i)
            {
                if (i == 0)
                    y_item_idx.push_back(0);
                else
                    y_item_idx.push_back(y_item_idx.back() + items.at(i - 1).GetHeight());
            }

            prev_rect = rect;
            updated = false;
            selected_item_updated = true;
        }

        // scroll
        if (auto_scroll && selected_item_updated)
        {
            const auto& item = items_ptr->at(selected_item_idx);
            auto item_y = y_item_idx.at(selected_item_idx);
            auto item_height = item.GetHeight();

            if (item_y < y + scroll_height)
                scroll_height -= y + scroll_height - item_y;
            else if (y + scroll_height + rect.height < item_y + item_height)
                scroll_height += item_y + item_height - (y + scroll_height + rect.height);

            selected_item_updated = false;
        }

        y += scroll_height;

        if (items_ptr->empty())
            return Span();

        // find item and span with y position
        auto y_item_idx_itr = std::prev(std::upper_bound(y_item_idx.begin(), y_item_idx.end(), y));
        size_t item_idx = y_item_idx_itr - y_item_idx.begin();
        auto local_y = y - *y_item_idx_itr;

        // find span
        const auto& item = items_ptr->at(item_idx);

        if (item.GetMap().empty())
            return Span();

        auto line_itr = item.GetMap().lower_bound(local_y);

        if (line_itr == item.GetMap().end())
        {
            return Span();
        }
        else if (line_itr->second.empty() || line_itr->first != local_y)
        {
            if (fill_span_bg_color)
            {
                if (enable_auto_appear)
                {
                    if (selected_item_idx == item_idx)
                        return Span(" ", auto_appear_selected);
                    else
                        return Span(" ", auto_appear_unselected);
                }
                else
                {
                    return Span(" ", std::prev(line_itr)->second.rbegin()->second.appear);
                }
            }
            return Span();
        }

        // x align
        auto line_width = item.GetLineWidth(local_y);
        int32_t actual_x = x;
        if (rect.width > line_width)
        {
            if (horiz_align == Align::Center)
                actual_x -= (rect.width - line_width) / 2;
            else if (horiz_align == Align::Right)
                actual_x -= rect.width - line_width;
        }
        
        if (actual_x < 0)
        {
            if (fill_span_bg_color)
            {
                if (enable_auto_appear)
                {
                    if (selected_item_idx == item_idx)
                        return Span(" ", auto_appear_selected);
                    else
                        return Span(" ", auto_appear_unselected);
                }

                return Span(" ", line_itr->second.begin()->second.appear);
            }
            return Span();
        }
        
        auto span_itr = std::prev(line_itr->second.upper_bound(actual_x));

        if (span_itr->first != actual_x || span_itr->second.str.empty())
        {
            if (fill_span_bg_color)
                return Span(" ", Appearance::Unchanged());
            return Span();
        }

        if (enable_auto_appear)
        {
            if (selected_item_idx == item_idx)
                return Span(span_itr->second.str, auto_appear_selected);
            else
                return Span(span_itr->second.str, auto_appear_unselected);
        }
        else
        {
            return span_itr->second;
        }
    }

    Border List::GetBorder() const
    {
        return border;
    }
}