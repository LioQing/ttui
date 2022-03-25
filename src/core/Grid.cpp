#include <ttui/core/Grid.hpp>

#include <cmath>
#include <numeric>
#include <list>

#include <ttui/core/Handle.hpp>
#include <ttui/core/Span.hpp>

namespace ttui
{
    Grid::Grid(const Rect& rect) : rect(rect)
    {
    }

    void ttui::Grid::SetDefinitions(const std::vector<Definition>& col_defs, const std::vector<Definition>& row_defs)
    {
        this->col_frames = std::vector<Frame>(col_defs.size());
        std::transform(col_defs.begin(), col_defs.end(), this->col_frames.begin(), 
        [](const Definition& def) { return Frame(def); });

        this->row_frames = std::vector<Frame>(row_defs.size());
        std::transform(row_defs.begin(), row_defs.end(), this->row_frames.begin(), 
        [](const Definition& def) { return Frame(def); });

        frames.resize(row_defs.size());
        for (auto& row : frames)
        {
            row.resize(col_defs.size());
        }

        require_col_def_update = true;
        require_row_def_update = true;
    }

    bool Grid::IsInitialized(size_t x, size_t y) const
    {
        return frames.at(y).at(x) != nullptr;
    }

    Widget& Grid::GetWidget(size_t x, size_t y)
    {
        return *frames.at(y).at(x);
    }

    Definition& Grid::GetColumnDefinition(size_t x)
    {
        require_col_def_update = true;
        return col_frames.at(x).def;
    }

    Definition& Grid::GetRowDefinition(size_t y)
    {
        require_row_def_update = true;
        return row_frames.at(y).def;
    }

    const Widget& Grid::GetWidget(size_t x, size_t y) const
    {
        return *frames.at(y).at(x);
    }

    const Definition& Grid::GetColumnDefinition(size_t x) const
    {
        return col_frames.at(x).def;
    }

    const Definition& Grid::GetRowDefinition(size_t y) const
    {
        return row_frames.at(y).def;
    }

    Rect Grid::GetWidgetRect(size_t x, size_t y) const
    {
        if (require_col_def_update)
            UpdateDef(Direction::Horizontal);

        if (require_row_def_update)
            UpdateDef(Direction::Vertical);

        Rect rect;

        rect.y = row_frames.at(y).pos;
        rect.height = row_frames.at(y).size;
        rect.x = col_frames.at(x).pos;
        rect.width = col_frames.at(x).size;

        return rect;
    }

    size_t Grid::GetColumnCount() const
    {
        return col_frames.size();
    }

    size_t Grid::GetRowCount() const
    {
        return row_frames.size();
    }

    Span Grid::GetSpan(uint16_t, uint16_t&, const Rect&) const
    {
        return Span();
    }

    bool Grid::Enabled() const
    {
        return false;
    }

    void Grid::Recursion(Handle& handle, const Rect& rect) const
    {
        this->rect = rect;
        require_col_def_update = true;
        require_row_def_update = true;
        
        for (size_t y = 0; y < GetRowCount(); ++y)
        for (size_t x = 0; x < GetColumnCount(); ++x)
        {
            if (IsInitialized(x, y))
                handle.Render(GetWidget(x, y), GetWidgetRect(x, y));
        }
    }

    void Grid::UpdateDef(Direction dir) const
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

        auto& frames = dir == Direction::Horizontal ? col_frames : row_frames;

        // required size
        uint16_t curr_pos = 0;
        std::list<size_t> auto_frames;

        for (size_t i = 0; i < frames.size(); ++i)
        {
            const auto& f = frames.at(i);
            f.pos = curr_pos + start_pos;

            if (curr_pos >= size)
            {
                f.size = 0;
                continue;
            }

            if (f.def.type == Definition::Type::Absolute)
            {
                f.size = f.def.absolute;
            }
            else if (f.def.type == Definition::Type::Relative)
            {
                f.size = f.def.relative * size;
            }
            else
            {
                f.size = 0;
                auto_frames.push_back(i);
            }

            if (f.size + f.pos > size + start_pos)
                f.size = size + start_pos - f.pos;

            curr_pos += f.size;
        }

        // distribute the remaining space to auto frames with the remainders
        if (auto_frames.size() > 0)
        {
            int32_t remaining = size - curr_pos;
            int32_t auto_size;
            int32_t remainders_size;

            while (remaining > 0 && !auto_frames.empty())
            {
                auto_size = remaining / auto_frames.size();
                remainders_size = remaining % auto_frames.size();
                remaining = 0;
                for (auto itr = auto_frames.begin(); itr != auto_frames.end();)
                {
                    auto& f = frames.at(*itr);
                    f.size += auto_size;
                    if (remainders_size > 0)
                    {
                        f.size += 1;
                        --remainders_size;
                    }

                    auto erase_itr = itr++;

                    // if frame is over max size, add it to remaining and set it to max size
                    if (f.def.fit.max != 0 && f.size > f.def.fit.max)
                    {
                        remaining += f.size - f.def.fit.max;
                        f.size = f.def.fit.max;
                        auto_frames.erase(erase_itr);
                    }

                    // if frame is under min size, subtract it from remaining and set it to min size
                    if (f.def.fit.min != 0 && f.size < f.def.fit.min)
                    {
                        remaining -= f.def.fit.min - f.size;
                        f.size = f.def.fit.min;
                        auto_frames.erase(erase_itr);
                    }
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

        if (dir == Direction::Horizontal)
            require_col_def_update = false;
        else
            require_row_def_update = false;
    }

    Grid::Frame::Frame() : def(Definition::Relative(1.f))
    {
    }

    Grid::Frame::Frame(const Definition& def) : def(def) 
    {
    }
}