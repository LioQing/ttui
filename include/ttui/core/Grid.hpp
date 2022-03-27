#pragma once

#include <vector>
#include <type_traits>
#include <algorithm>
#include <memory>

#include <ttui/core/Direction.hpp>
#include <ttui/core/Widget.hpp>
#include <ttui/core/Definition.hpp>
#include <ttui/core/Rect.hpp>

namespace ttui
{
    struct Grid : Widget
    {
        Grid() = default;
        Grid(Grid&& other) = default;

        Grid(const Rect& rect);

        void SetDefinitions(const std::vector<Definition>& col_defs, const std::vector<Definition>& row_defs);

        bool IsInitialized(size_t x, size_t y) const;

        Widget& GetWidget(size_t x, size_t y);

        Definition& GetColumnDefinition(size_t x);
        Definition& GetRowDefinition(size_t y);

        const Widget& GetWidget(size_t x, size_t y) const;

        const Definition& GetColumnDefinition(size_t x) const;
        const Definition& GetRowDefinition(size_t y) const;

        template <typename TWidget>
        void SetWidget(size_t x, size_t y, TWidget&& widget);

        Rect GetWidgetRect(size_t x, size_t y) const;

        size_t GetColumnCount() const;
        size_t GetRowCount() const;

        Span GetSpan(uint16_t, uint16_t, const Rect&) const override;
        bool Enabled() const override;
        void Recursion(Handle& handle, const Rect& rect) const override;

    private:

        using WidgetPtr = std::unique_ptr<Widget>;

        void UpdateDef(Direction dir) const;

        struct Frame
        {
            Frame();
            Frame(const Definition& def);
            Frame(const Frame& other) = default;
            
            Definition def;

            mutable uint16_t pos;
            mutable uint16_t size;
        };

        mutable Rect rect;

        std::vector<std::vector<WidgetPtr>> frames;
        std::vector<Frame> col_frames;
        std::vector<Frame> row_frames;
        mutable bool require_col_def_update = false;
        mutable bool require_row_def_update = false;
    };
}

template <typename TWidget>
void ttui::Grid::SetWidget(size_t x, size_t y, TWidget&& widget)
{
    using NonRefTWidget = typename std::remove_reference<TWidget>::type;

    static_assert(std::is_base_of<Widget, NonRefTWidget>::value, "widget is not base of Widget");

    frames.at(y).at(x) = std::unique_ptr<NonRefTWidget>(new NonRefTWidget(std::forward<TWidget>(widget)));
}