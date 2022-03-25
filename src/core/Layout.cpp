#include <ttui/core/Layout.hpp>

#include <cmath>
#include <numeric>

#include <ttui/core/Handle.hpp>
#include <ttui/core/Span.hpp>

namespace ttui
{
    Layout::Layout(Direction dir, const Rect& rect) : dir(dir), rect(rect)
    {
    }

    void Layout::SetDirection(Direction dir)
    {
        this->dir = dir;
        require_def_update = true;
    }

    void ttui::Layout::SetDefinitions(const std::vector<Definition>& v)
    {
        frames.resize(v.size());

        for (size_t i = 0; i < v.size(); ++i)
        {
            frames.at(i).def = v.at(i);
        }

        require_def_update = true;
    }

    bool Layout::IsInitialized(size_t index) const
    {
        return frames.at(index).widget != nullptr;
    }

    Widget& Layout::GetWidget(size_t index)
    {
        return *frames.at(index).widget;
    }

    Definition& Layout::GetDefinition(size_t index)
    {
        require_def_update = true;
        return frames.at(index).def;
    }

    const Widget& Layout::GetWidget(size_t index) const
    {
        return *frames.at(index).widget;
    }

    const Definition& Layout::GetDefinition(size_t index) const
    {
        return frames.at(index).def;
    }

    Rect Layout::GetWidgetRect(size_t index) const
    {
        if (require_def_update)
            UpdateDef();

        auto rect = this->rect;

        if (dir == Direction::Vertical)
        {
            rect.y = frames.at(index).pos;
            rect.height = frames.at(index).size;
        }
        else
        {
            rect.x = frames.at(index).pos;
            rect.width = frames.at(index).size;
        }

        return rect;
    }

    size_t Layout::GetWidgetCount() const
    {
        return frames.size();
    }

    Span Layout::GetSpan(uint16_t, uint16_t&, const Rect&) const
    {
        return Span();
    }

    bool Layout::Enabled() const
    {
        return false;
    }

    void Layout::Recursion(Handle& handle, const Rect& rect) const
    {
        this->rect = rect;
        require_def_update = true;
        
        for (size_t i = 0; i < GetWidgetCount(); ++i)
        {
            if (IsInitialized(i))
                handle.Render(GetWidget(i), GetWidgetRect(i));
        }
    }

    void Layout::UpdateDef() const
    {
        if (frames.empty())
            return;

        auto start_pos = rect.x;
        auto size = rect.width;
        if (dir == Direction::Vertical)
        {
            start_pos = rect.y;
            size = rect.height;
        }

        // required size
        uint16_t curr_pos = 0;
        std::vector<size_t> available;

        for (size_t i = 0; i < frames.size(); ++i)
        {
            const auto& f = frames.at(i);
            f.pos = curr_pos + start_pos;

            if (curr_pos >= size)
            {
                f.size = 0;
                continue;
            }

            if (f.def.type == Definition::Type::Length)
            {
                f.size = f.def.length;
            }
            else if (f.def.type == Definition::Type::Relative)
            {
                f.size = f.def.relative * size;
            }
            else
            {
                f.size = 0;
                available.push_back(i);
            }

            if (f.size + f.pos > size + start_pos)
                f.size = size + start_pos - f.pos;

            curr_pos += f.size;
        }

        // if there is available, distribute the remaining space including the reminader
        // else distribute the space to the last one
        if (!available.empty())
        {
            auto remaining = size - curr_pos;
            auto remaining_distr = remaining / available.size();
            auto remaining_remainder = remaining % available.size();

            for (size_t i = 0; i < available.size(); ++i)
            {
                auto& f = frames.at(available.at(i));
                f.size += remaining_distr;

                // keep the size <= max or >= min and distribute the extra space to other frames
                if (f.def.type == Definition::Type::Max)
                {
                    if (f.size > f.def.max)
                    {
                        f.size = f.def.max;
                        remaining -= f.size;
                        remaining_remainder += 1;
                    }
                }
                else if (f.def.type == Definition::Type::Min)
                {
                    if (f.size < f.def.min)
                    {
                        f.size = f.def.min;
                        remaining -= f.size;
                        remaining_remainder += 1;
                    }
                }

                if (remaining_remainder > 0)
                {
                    f.size += 1;
                    --remaining_remainder;
                }
            }
        }

        // re evaluate the pos of the frames
        curr_pos = 0;
        for (auto& f : frames)
        {
            f.pos = curr_pos + start_pos;
            
            if (f.pos > size + start_pos)
            {
                f.size = 0;
            }
            else if (f.pos + f.size > size + start_pos)
            {
                f.size = size + start_pos - f.pos;
            }

            curr_pos += f.size;
        }

        // if there is remaining space, distribute it to the last frame
        if (size - curr_pos > 0)
        {
            frames.back().size += size - curr_pos;
        }

        require_def_update = false;
    }

    Layout::Frame::Frame() : widget(nullptr), def(Definition::Relative(1.f))
    {
    }
}