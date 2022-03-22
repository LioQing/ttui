#pragma once

#include <cstdint>

namespace ttui
{
    struct Color
    {
        Color() = default;
        Color(const Color&) = default;

        static Color Black();
        static Color Red();
        static Color Green();
        static Color Yellow();
        static Color Blue();
        static Color Magenta();
        static Color Cyan();
        static Color White();
        static Color BrightBlack();
        static Color BrightRed();
        static Color BrightGreen();
        static Color BrightYellow();
        static Color BrightBlue();
        static Color BrightMagenta();
        static Color BrightCyan();
        static Color BrightWhite();
        static Color Reset();
        static Color RGB(uint32_t rgb);
        static Color RGB(uint8_t r, uint8_t g, uint8_t b);
        static Color Color8bit(uint8_t col);

        enum class Type
        {
            Black,
            Red,
            Green,
            Yellow,
            Blue,
            Magenta,
            Cyan,
            White,
            BrightBlack,
            BrightRed,
            BrightGreen,
            BrightYellow,
            BrightBlue,
            BrightMagenta,
            BrightCyan,
            BrightWhite,
            Reset,
            RGB,
            Color8bit,
        };

        Type type = Type::Reset;

        union
        {
            uint32_t rgb;
            uint8_t color_8bit;
        };

        bool operator==(const Color& other) const;
        bool operator!=(const Color& other) const;
    };
}