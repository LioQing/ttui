#pragma once

#include <vector>
#include <type_traits>
#include <algorithm>
#include <memory>

#include <ttui/Direction.hpp>
#include <ttui/Widget.hpp>
#include <ttui/Definition.hpp>

namespace ttui
{
    struct Layout
    {
        void SetDirection(Direction direction);

        template <typename TContainer, typename TValue = typename TContainer::value_type>
        void SetDefinitions(const TContainer& container)
        {
            static_assert(std::is_same<TValue, Definition>::value, "value type is not Definition");

            frames.resize(container.size());
            std::transform(container.begin(), container.end(), frames.begin(),
            [](const Definition& definition)
            {
                WidgetInfo info;
                info.definition = definition;
                return info;
            });
        }

        Widget& GetWidget(size_t index);
        Definition& GetDefinition(size_t index);

    private:

        struct WidgetInfo
        {
            std::unique_ptr<Widget> widget;
            Definition definition;
        };

        Direction direction = Direction::Horizontal;

        std::vector<WidgetInfo> frames;
    };
}