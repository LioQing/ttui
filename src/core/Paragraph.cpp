#include <ttui/core/Paragraph.hpp>

namespace ttui
{
    Paragraph::Paragraph(const Map& map) : map(map)
    {
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