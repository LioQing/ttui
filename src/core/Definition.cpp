#include <ttui/core/Definition.hpp>

namespace ttui
{
    Definition Definition::Proportion(uint32_t prop)
    {
        Definition def;
        def.type = Definition::Type::Proportion;
        def.proportion = prop;

        return def;
    }

    Definition Definition::Percentage(float perc)
    {
        Definition def;
        def.type = Definition::Type::Percentage;
        def.percentage = perc;

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