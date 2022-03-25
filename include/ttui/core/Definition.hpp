#pragma once

#include <cstdint>

namespace ttui
{
    /**
     * @brief A structure for defining Widgets in Layout.
     * 
     */
    struct Definition
    {
        Definition(const Definition&) = default;

        /**
         * @brief Define layout with relative size. Example, { Percentage(0.505f) } will get 50.5% of the total layout size.
         * 
         * @param perc The percentage
         * @return Definition The definition
         */
        static Definition Relative(float perc);

        /**
         * @brief Define layout with absolute length. Example, { Absolute(5) } will get the length of 5 characters.
         * 
         * @param absolute The absolute length
         * @return Definition The definition
         */
        static Definition Absolute(uint16_t absolute);

        /**
         * @brief Define the layout with auto resizing length.
         * 
         * @return Definition The definition
         */
        static Definition Fit(uint16_t min = 0, uint16_t max = 0);

        /**
         * @brief Enum type of definition.
         * 
         */
        enum class Type
        {
            Relative, Absolute, Fit,
        };

        Type type = Type::Fit;

        union
        {
            float relative;
            uint16_t absolute;
            struct { uint16_t min, max; } fit;
        };
    
    private:

        Definition() = default;
    };
}