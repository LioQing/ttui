#pragma once

#include <vector>

#include <ttui/core/Widget.hpp>
#include <ttui/core/Paragraph.hpp>
#include <ttui/core/Border.hpp>
#include <ttui/core/Appearance.hpp>
#include <ttui/core/Align.hpp>
#include <ttui/core/Wrap.hpp>
#include <ttui/core/Rect.hpp>

namespace ttui
{
    struct List : Widget
    {
        List() = default;
        List(const List&) = default;

        List(const std::vector<Paragraph>& items, Border border = Border::Single(), Wrap wrap = Wrap::None);

        void SetItems(const std::vector<Paragraph>& items);
        const std::vector<Paragraph>& GetItems() const;

        void SetItemAt(uint16_t index, const Paragraph& item);
        const Paragraph& GetItemAt(uint16_t index) const;

        void PushBack(const Paragraph& item);
        size_t GetSize() const;

        void SetBorder(const Border& border);

        void SetHorizAlign(Align align);
        Align GetHorizAlign() const;
        
        void SetWrap(Wrap wrap);
        Wrap GetWrap() const;

        void SetScrollHeight(uint16_t height);
        uint16_t GetScrollHeight() const;

        /**
         * @brief Enable or disable the filling of the background of whole line with that of the last span.
         * 
         * @param enabled Enable or not
         */
        void SetFillSpanBackgroundColor(bool enabled);

        /**
         * @brief Check whether is the background of whole line filled with that of the last span.
         * 
         * @return bool Enabled or not
         */
        bool IsFillSpanBackgroundColor() const;

        /**
         * @brief Set the index of the item selected, only have effect when auto appearance is enabled.
         * 
         * @param item_index The index of the item
         */
        void SetSelectedItemIndex(size_t item_index);

        /**
         * @brief Get the index of the item selected.
         * 
         * @return size_t The index of the item
         */
        size_t GetSelectedItemIndex() const;

        /**
         * @brief Enable or disable automatically setting appearance for items.
         * 
         * @param enabled Enable or not
         */
        void SetEnableAutoAppearance(bool enabled);

        /**
         * @brief Check whether is automatically setting appearance for items enabled.
         * 
         * @return bool Enabled or not
         */
        bool IsAutoAppearanceEnabled() const;

        /**
         * @brief Set the unselected and selected appearances for auto appearance.
         * 
         * @param unselected The unselected appearance
         * @param selected The unselected appearance
         */
        void SetAutoAppearance(const Appearance& unselected, const Appearance& selected);

        /**
         * @brief Get the appearance for either selected or unselected item.
         * 
         * @param selected Selected or unselected
         * @return const Appearance& The appearance
         */
        const Appearance& GetAutoAppearance(bool selected) const;

        const std::vector<Paragraph>& GetWrappedItems(uint16_t width) const;

        Span GetSpan(uint16_t y, uint16_t x, const Rect& rect) const override;
        Border GetBorder() const override;
    
    private:

        std::vector<Paragraph> items;
        Border border = Border::Single();
        Align horiz_align = Align::Left;
        Wrap wrap = Wrap::None;

        mutable uint16_t scroll_height = 0;
        bool auto_scroll = true;

        bool fill_span_bg_color = true;

        bool enable_auto_appear = false;
        size_t selected_item_idx = 0;
        Appearance auto_appear_unselected;
        Appearance auto_appear_selected = Appearance(Color::White(), Color::BrightBlack());

        mutable bool updated = true;
        mutable bool selected_item_updated = true;
        mutable Rect prev_rect = Rect();
        mutable std::vector<Paragraph> wrapped_items;

        mutable std::vector<uint16_t> y_item_idx;
    };
}