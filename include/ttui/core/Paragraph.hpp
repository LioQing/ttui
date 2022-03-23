#pragma once

#include <cstdint>
#include <map>
#include <vector>

#include <ttui/core/Span.hpp>

namespace ttui
{
    struct Paragraph
    {
        using Map = std::map<uint16_t, std::map<uint16_t, Span>>;

        Paragraph() = default;
        Paragraph(const Paragraph&) = default;

        Paragraph(const Map& map);

        bool HasLine(uint16_t line_no) const;
        bool AddLine(uint16_t line_no);
        bool AddLine(uint16_t line_no, const Span& span);
        bool AddLine(uint16_t line_no, const std::vector<Span>& spans);
        bool AddLine(uint16_t line_no, const std::map<uint16_t, Span>& spans);
        void EraseLine(uint16_t line_no);
        const std::map<uint16_t, Span>& GetLine(uint16_t line_no) const;

        bool HasSpan(uint16_t line_no, uint16_t x_coord) const;
        std::pair<uint16_t, uint16_t> TestSpan(uint16_t line_no, uint16_t x_coord, const Span& span) const;
        bool AddSpan(uint16_t line_no, uint16_t x_coord, const Span& span);
        void EraseSpan(uint16_t line_no, uint16_t x_coord);
        const Span& GetSpan(uint16_t line_no, uint16_t x_coord) const;

        void SetMap(const Map& map);
        const Map& GetMap() const;

    private:

        Map map;
    };
}