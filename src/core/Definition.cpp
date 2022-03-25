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

    Definition Definition::Absolute(uint16_t absolute)
    {
        Definition def;
        def.type = Definition::Type::Absolute;
        def.absolute = absolute;

        return def;
    }

    Definition Definition::Fit(uint16_t min, uint16_t max)
    {
        Definition def;
        def.type = Definition::Type::Fit;
        def.fit.min = min;
        def.fit.max = max;

        return def;
    }
}