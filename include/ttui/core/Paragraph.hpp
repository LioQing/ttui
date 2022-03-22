#pragma once

#include <cstdint>
#include <map>

#include <ttui/core/Span.hpp>

namespace ttui
{
    struct Paragraph
    {
        using Map = std::map<uint16_t, std::map<uint16_t, Span>>;

        Paragraph() = default;
        Paragraph(const Paragraph&) = default;

        Paragraph(const Map& map);

        void SetMap(const Map& map);
        const Map& GetMap() const;

    private:

        Map map;
    };
}