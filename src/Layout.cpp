#include <ttui/Layout.hpp>

namespace ttui
{
    void Layout::SetDirection(Direction direction)
    {
        this->direction = direction;
    }

    Widget& Layout::GetWidget(size_t index)
    {
        return *frames.at(index).widget;
    }

    Definition& Layout::GetDefinition(size_t index)
    {
        return frames.at(index).definition;
    }
}