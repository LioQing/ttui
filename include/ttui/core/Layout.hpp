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
    struct Layout : Widget
    {
        Layout() = default;
        Layout(Layout&& other) = default;

        Layout(Direction dir, const Rect& rect);

        void SetDirection(Direction dir);

        void SetDefinitions(const std::vector<Definition>& container);

        bool IsInitialized(size_t index) const;

        Widget& GetWidget(size_t index);
        Definition& GetDefinition(size_t index);

        const Widget& GetWidget(size_t index) const;
        const Definition& GetDefinition(size_t index) const;

        template <typename TWidget>
        void SetWidget(size_t index, TWidget&& widget);

        Rect GetWidgetRect(size_t index) const;
        size_t GetWidgetCount() const;

        Span GetSpan(uint16_t, uint16_t&, const Rect&) const override;
        bool Enabled() const override;
        void Recursion(Handle& handle, const Rect& rect) const override;

    private:

        void UpdateDef() const;

        struct Frame
        {
            Frame();
            Frame(Frame&& other) = default;
            
            std::unique_ptr<Widget> widget;
            
            Definition def;

            mutable uint16_t pos;
            mutable uint16_t size;
        };

        Direction dir = Direction::Horizontal;
        mutable Rect rect;

        std::vector<Frame> frames;
        mutable bool require_def_update = false;
    };
}

template <typename TWidget>
void ttui::Layout::SetWidget(size_t index, TWidget&& widget)
{
    using NonRefTWidget = typename std::remove_reference<TWidget>::type;

    static_assert(std::is_base_of<Widget, NonRefTWidget>::value, "widget is not base of Widget");

    frames.at(index).widget = std::unique_ptr<NonRefTWidget>(new NonRefTWidget(std::forward<TWidget>(widget)));
}