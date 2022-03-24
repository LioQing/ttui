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
         * @brief Define layout with length. Example, { Length(5) } will get the length of 5 characters.
         * 
         * @param length The length
         * @return Definition The definition
         */
        static Definition Length(uint16_t length);

        /**
         * @brief Define layout with max length.
         * 
         * @param max The max length
         * @return Definition The definition
         */
        static Definition Max(uint16_t max);
        
        /**
         * @brief Define layout with min length.
         * 
         * @param max The min length
         * @return Definition The definition
         */
        static Definition Min(uint16_t min);

        /**
         * @brief Enum type of definition.
         * 
         */
        enum class Type
        {
            Relative, Length, Max, Min,
        };

        Type type;

        union
        {
            float relative;
            uint16_t length;
            uint16_t max;
            uint16_t min;
        };
    
    private:

        Definition() = default;
    };
}