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
        auto pos = rect.x;
        auto size = rect.width;
        if (dir == Direction::Vertical)
        {
            pos = rect.y;
            size = rect.height;
        }

        // required size
        uint16_t curr_pos = 0;
        std::vector<size_t> available;
        uint16_t min_sum = 0;

        for (size_t i = 0; i < frames.size(); ++i)
        {
            const auto& f = frames.at(i);
            f.pos = curr_pos + pos;

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
            else if (f.def.type == Definition::Type::Min)
            {
                f.size = f.def.min;

                min_sum += f.size;
                available.push_back(i);
            }
            else if (f.def.type == Definition::Type::Max)
            {
                f.size = 0;

                available.push_back(i);
            }

            if (f.size > size - f.pos)
                f.size = size - f.pos;

            curr_pos += f.size;
        }

        if (frames.back().pos + frames.back().size < pos + size)
        {
            frames.back().size = pos + size - frames.back().pos;
            require_def_update = false;
            return;
        }

        if (frames.size() == 1)
        {
            frames.front().size = pos + size - frames.front().pos;
            require_def_update = false;
            return;
        }

        // auto allocated size
        // uint16_t distr_value = size - curr_pos;

        // std::sort(available.begin(), available.end(), 
        // [&](size_t a, size_t b){ return frames.at(a).size > frames.at(b).size; });
        
        // size_t i = 0;
        // uint16_t target;
        // for (; i < available.size(); ++i)
        // {
        //     target = (distr_value + min_sum) / (available.size() - i);
        //     if (target > frames.at(available.at(i)).size)
        //         break;
            
        //     min_sum -= frames.at(available.at(i)).size;
        // }

        // distr_value -= target * (i + 1);
        // for (size_t j = i; j < available.size(); ++j)
        // {
        //     frames.at(available.at(j)).size = target + (j - i < distr_value);
        // }

        // curr_pos = frames.front().size;
        // for (size_t i = 1; i < frames.size(); ++i)
        // {
        //     frames.at(i).pos = curr_pos;
        //     curr_pos += frames.at(i).size;
        // }

        // frames.back().size = size - frames.back().pos;

        require_def_update = false;
    }

    Layout::Frame::Frame() : widget(nullptr), def(Definition::Relative(1.f))
    {
    }
}