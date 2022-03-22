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
         * @brief Define layout with proportion. Example, { Proportion(1), Proportion(2) } will get 1:2.
         * 
         * @param prop The proportion
         * @return Definition The definition
         */
        static Definition Proportion(uint32_t prop);

        /**
         * @brief Define layout with percentage. Example, { Percentage(50.5f) } will get 50.5%.
         * 
         * @param perc The percentage
         * @return Definition The definition
         */
        static Definition Percentage(float perc);

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
            Proportion, Percentage, Length, Max, Min,
        };

        Type type;

        union
        {
            uint32_t proportion;
            float percentage;
            uint16_t length;
            uint16_t max;
            uint16_t min;
        };
    
    private:

        Definition() = default;
    };
}