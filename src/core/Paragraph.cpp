#include <ttui/core/Paragraph.hpp>

namespace ttui
{
    Paragraph::Paragraph(const Map& map) : map(map)
    {
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
            result.first->second.emplace(next_x, s);
            next_x = s.str.size();
        }

        return result.second;
    }

    bool Paragraph::AddLine(uint16_t line_no, const std::map<uint16_t, Span>& spans)
    {
        auto result = map.emplace(line_no, std::map<uint16_t, Span>());
        if (!result.second)
            return result.second;

        for (auto itr = spans.begin(); itr != spans.end(); ++itr)
        {
            auto next_itr = std::next(itr);
            if (next_itr == spans.end())
            {
                result.first->second.emplace(*itr);
            }
            else if (itr->second.str.size() > (size_t)next_itr->first)
            {
                auto insert_span = Span(itr->second.str.substr(0, next_itr->first - itr->first), itr->second.appear);
                result.first->second.emplace(itr->first, insert_span);
            }
            else
            {
                result.first->second.emplace(*itr);
            }
        }

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

    bool Paragraph::HasSpan(uint16_t line_no, uint16_t x_coord) const
    {
        return GetLine(line_no).find(x_coord) != GetLine(line_no).end();
    }

    std::pair<uint16_t, uint16_t> Paragraph::TestSpan(uint16_t line_no, uint16_t x_coord, const Span& span) const
    {
        const auto& line = GetLine(line_no);

        auto next_span = line.lower_bound(x_coord);
        auto next_dist = 0;
        if (next_span != line.end())
            next_dist = next_span->first - x_coord - span.str.size();

        auto prev_dist = 0;
        if (next_span != line.begin())
        {
            auto prev_span = std::prev(next_span);
            prev_dist = x_coord - prev_span->first - prev_span->second.str.size();
        }

        return std::make_pair(prev_dist, next_dist);
    }

    bool Paragraph::AddSpan(uint16_t line_no, uint16_t x_coord, const Span& span)
    {
        if (!HasLine(line_no))
            AddLine(line_no);

        const auto& line = GetLine(line_no);
        if (HasSpan(line_no, x_coord))
            return false;
        
        auto dists = TestSpan(line_no, x_coord, span);
        auto result = map.at(line_no).emplace(x_coord, span);
        if (!result.second)
            return false;
        
        if (dists.second < 0)
        {
            result.first->second.str.resize(result.first->second.str.size() - dists.second);
        }

        if (dists.first < 0 && result.first != line.begin())
        {
            auto prev_span = std::prev(result.first);
            prev_span->second.str.resize(prev_span->second.str.size() - dists.first);
        }

        return true;
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
}