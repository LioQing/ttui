#include <ttui/core/Paragraph.hpp>

#include <ttui/core/MBString.hpp>

namespace ttui
{
    Paragraph::Paragraph(const Map& map) : map(map)
    {
    }

    Paragraph::Paragraph(const std::string& str, const Appearance& appear)
    {
        SetSpan(0, 0, Span(str, appear));
    }

    void Paragraph::SetAppearance(const Appearance& appear)
    {
        for (auto& l : map)
        {
            for (auto& s : l.second)
            {
                s.second.appear = appear;
            }
        }
    }

    bool Paragraph::HasLine(uint16_t line_no) const
    {
        return GetMap().find(line_no) != GetMap().end();
    }

    bool Paragraph::AddLine(uint16_t line_no)
    {
        return map.emplace(line_no, std::map<uint16_t, Span>()).second;
    }

    bool Paragraph::AddLine(uint16_t line_no, const Span& span)
    {
        return map.emplace(line_no, std::map<uint16_t, Span>{ { 0, span } }).second;
    }

    bool Paragraph::AddLine(uint16_t line_no, const std::vector<Span>& spans)
    {
        auto result = map.emplace(line_no, std::map<uint16_t, Span>());
        if (!result.second)
            return result.second;

        uint16_t next_x = 0;
        for (const auto& s : spans)
        {
            AddSpan(line_no, next_x, s);
            next_x = MBString::Size(s.str);
        }

        return result.second;
    }

    bool Paragraph::AddLine(uint16_t line_no, const std::map<uint16_t, Span>& spans)
    {
        auto result = map.emplace(line_no, std::map<uint16_t, Span>());
        if (!result.second)
            return result.second;

        for (const auto& s : spans)
            AddSpan(line_no, s.first, s.second);

        return result.second;
    }

    void Paragraph::EraseLine(uint16_t line_no)
    {
        map.erase(line_no);
    }

    const std::map<uint16_t, Span>& Paragraph::GetLine(uint16_t line_no) const
    {
        return GetMap().at(line_no);
    }

    bool Paragraph::SwapLines(uint16_t line_no_a, uint16_t line_no_b)
    {
        auto a_itr = map.find(line_no_a);
        auto b_itr = map.find(line_no_b);
        if (a_itr == map.end() || b_itr == map.end())
            return false;
        
        std::swap(a_itr->second, b_itr->second);
        return true;
    }
    
    void Paragraph::SetLineAppearance(uint16_t line_no, const Appearance& appear)
    {
        for (auto& s : map.at(line_no))
        {
            s.second.appear = appear;
        }
    }

    uint16_t Paragraph::GetLineWidth(uint16_t line_no) const
    {
        if (map.at(line_no).empty())
            return 0;
        const auto& last = map.at(line_no).rbegin();
        return last->first + MBString::Size(last->second.str) - map.at(line_no).begin()->first;
    }

    bool Paragraph::HasSpan(uint16_t line_no, uint16_t x_coord) const
    {
        return HasLine(line_no) && GetLine(line_no).find(x_coord) != GetLine(line_no).end();
    }

    std::pair<uint16_t, uint16_t> Paragraph::TestSpan(uint16_t line_no, uint16_t x_coord, const Span& span) const
    {
        const auto& line = GetLine(line_no);

        auto next_span = line.lower_bound(x_coord);
        auto next_dist = 0;
        if (next_span != line.end())
            next_dist = next_span->first - x_coord - MBString::Size(span.str);

        auto prev_dist = 0;
        if (next_span != line.begin())
        {
            auto prev_span = std::prev(next_span);
            prev_dist = x_coord - prev_span->first - MBString::Size(prev_span->second.str);
        }

        return std::make_pair(prev_dist, next_dist);
    }

    bool Paragraph::AddSpan(uint16_t line_no, uint16_t x_coord, const Span& span)
    {
        if (!HasLine(line_no))
            AddLine(line_no);

        if (HasSpan(line_no, x_coord))
            return false;
        
        auto result = map.at(line_no).emplace(x_coord, span);
        if (!result.second)
            return result.second;
        
        SetSpan(line_no, x_coord, span);

        return result.second;
    }

    void Paragraph::SetSpan(uint16_t line_no, uint16_t x_coord, const Span& span)
    {
        if (span.str.find('\n') != std::string::npos)
        {
            // new line count
            size_t count = 0;
            for (auto c : span.str)
                count += c == '\n';

            // shift lines below
            if (!HasLine(line_no))
                AddLine(line_no);
            
            for (auto itr = map.rbegin(); itr != map.rend(); ++itr)
            {
                if (itr->first == line_no)
                    break;
                
                if (map.find(itr->first + count) == map.end())
                    map.emplace(itr->first + count, std::map<uint16_t, Span>());

                SwapLines(itr->first + count, itr->first);

                itr->second.clear();
            }
            
            // set spans
            auto rfindnl = [&](int pos) -> size_t
            {
                for (int i = pos; i >= 0; --i)
                    if (span.str.at(i) == '\n')
                        return i;
                return std::string::npos;
            };

            size_t s, e, i;
            for (
                s = span.str.size(), e = rfindnl(s - 1), i = count;
                e != std::string::npos;
                s = e, e = rfindnl(s - 1), --i
                )
            {
                if (map.find(line_no + i) == map.end())
                    map.emplace(line_no + i, std::map<uint16_t, Span>({ { 0, Span(span.str.substr(e + 1, s - e - 1), span.appear) } }));
                else
                    map.at(line_no + i) = { { 0, Span(span.str.substr(e + 1, s - e - 1), span.appear) } };
            }

            if (s != 0)
                map.at(line_no) = { { 0, Span(span.str.substr(0, s), span.appear) } };

            return;
        }

        if (!HasSpan(line_no, x_coord))
        {
            AddSpan(line_no, x_coord, span);
            return;
        }

        auto& in_span = map.at(line_no).at(x_coord);
        in_span = span;

        auto dists = TestSpan(line_no, x_coord, span);

        if (dists.second < 0)
        {
            MBString::Substr(in_span.str, 0, MBString::Size(in_span.str) - dists.second);
        }

        auto itr = map.at(line_no).find(x_coord);
        if (dists.first < 0 && itr != map.at(line_no).begin())
        {
            auto prev_span = std::prev(itr);
            MBString::Substr(prev_span->second.str, 0, MBString::Size(prev_span->second.str) - dists.first);
        }
    }

    void Paragraph::EraseSpan(uint16_t line_no, uint16_t x_coord)
    {
        map.at(line_no).erase(x_coord);
    }

    const Span& Paragraph::GetSpan(uint16_t line_no, uint16_t x_coord) const
    {
        return GetLine(line_no).at(x_coord);
    }

    void Paragraph::SetMap(const Map& map)
    {
        this->map = map;
    }

    const Paragraph::Map& Paragraph::GetMap() const
    {
        return map;
    }

    uint16_t Paragraph::GetHeight() const
    {
        if (map.empty())
            return 0;
        
        return map.rbegin()->first - map.begin()->first + 1;
    }

    void Paragraph::EraseEmptySpans()
    {
        // erase spans with empty str
        for (auto line_itr = map.begin(); line_itr != map.end();)
        {
            for (auto itr = line_itr->second.begin(); itr != line_itr->second.end();)
            {
                auto copy_itr = itr++;
                if (itr->second.str.empty())
                    line_itr->second.erase(copy_itr);
            }

            auto copy_itr = line_itr++;
            if (line_itr->second.empty())
                map.erase(copy_itr);
        }
    }

    Paragraph Paragraph::Wrapped(Wrap wrap, uint16_t width) const
    {
        Paragraph wrapped;

        if      (wrap == Wrap::Word)                wrapped = WordWrapped(width, true);
        if      (wrap == Wrap::WordWithoutSpace)    wrapped = WordWrapped(width, false);
        else if (wrap == Wrap::Span)                wrapped = SpanWrapped(width);

        return wrapped;
    }

    Paragraph Paragraph::SpanWrapped(uint16_t width) const
    {
        Paragraph wrapped_para;
        Map& wrapped_map = wrapped_para.map;

        int32_t line_offset = 0;
        int32_t x_offset = 0;
        int32_t y_offset = 0;

        for (const auto& line : map)
        {
            x_offset = 0;
            y_offset = 0;

            wrapped_map.emplace(line.first + line_offset, std::map<uint16_t, Span>());

            for (auto itr = line.second.begin(); itr != line.second.end(); ++itr)
            {
                auto pos = itr->first + x_offset - y_offset * width;
                const auto& span = itr->second;

                if (MBString::Size(span.str) <= width && pos + MBString::Size(span.str) > width)
                {
                    auto add_y = 1;
                    while (pos + (int32_t)MBString::Size(span.str) > width * (add_y + 1))
                        ++add_y;

                    y_offset += add_y;
                    wrapped_map.emplace(line.first + line_offset + y_offset, std::map<uint16_t, Span>());

                    if (pos < width * add_y)
                    {
                        x_offset += width * add_y - pos;
                    }

                    pos = itr->first + x_offset - y_offset * width;
                }
                else if (MBString::Size(span.str) > width)
                {
                    x_offset -= pos + MBString::Size(span.str) - width;
                }
                
                wrapped_map.at(line.first + line_offset + y_offset).emplace(pos, span);
            }

            line_offset += y_offset;
        }

        return wrapped_para;
    }

    Paragraph Paragraph::WordWrapped(uint16_t width, bool include_space) const
    {
        Paragraph wrapped_para(map);
        Map& wrapped_map = wrapped_para.map;

        auto Split = [include_space](const std::string& str) -> std::map<uint16_t, std::string>
        {
            std::map<uint16_t, std::string> words;

            auto last_itr = str.begin();
            for (auto itr = last_itr; itr != str.end(); ++itr)
            {
                if (*itr == ' ')
                {
                    std::string word = std::string(last_itr, itr + include_space);
                    if (!word.empty())
                    {
                        words.emplace(last_itr - str.begin(), word);
                    }
                    last_itr = itr + 1;
                }
            }

            if (last_itr != str.end())
                words.emplace(std::distance(str.begin(), last_itr), std::string(last_itr, str.end()));

            return words;
        };

        // for each line in wrapped_word, for each span in line, split the span into words
        for (auto& line : wrapped_map)
        {
            for (auto& span : line.second)
            {
                auto words = Split(span.second.str);

                // for each word in words, if the word is longer than width, split the word into multiple spans
                for (auto& word : words)
                    wrapped_para.SetSpan(line.first, span.first + word.first, Span(word.second, span.second.appear));
            }
        }

        return wrapped_para.SpanWrapped(width);
    }
}