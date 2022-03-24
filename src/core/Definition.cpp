#include <ttui/core/Definition.hpp>

namespace ttui
{
    Definition Definition::Relative(float perc)
    {
        Definition def;
        def.type = Definition::Type::Relative;
        def.relative = perc;

        return def;
    }

    Definition Definition::Length(uint16_t length)
    {
        Definition def;
        def.type = Definition::Type::Length;
        def.length = length;

        return def;
    }

    Definition Definition::Max(uint16_t max)
    {
        Definition def;
        def.type = Definition::Type::Max;
        def.max = max;

        return def;
    }

    Definition Definition::Min(uint16_t min)
    {
        Definition def;
        def.type = Definition::Type::Min;
        def.min = min;

        return def;
    }
}