#include <ttui/core/Color.hpp>

namespace ttui
{
    Color Color::Black()
    {
        Color color;
        color.type = Color::Type::Black;
        
        return color;
    }

    Color Color::Red()
    {
        Color color;
        color.type = Color::Type::Red;
        
        return color;
    }

    Color Color::Green()
    {
        Color color;
        color.type = Color::Type::Green;

        return color;
    }

    Color Color::Yellow()
    {
        Color color;
        color.type = Color::Type::Yellow;

        return color;
    }

    Color Color::Blue()
    {
        Color color;
        color.type = Color::Type::Blue;

        return color;
    }

    Color Color::Magenta()
    {
        Color color;
        color.type = Color::Type::Magenta;

        return color;
    }

    Color Color::Cyan()
    {
        Color color;
        color.type = Color::Type::Cyan;

        return color;
    }

    Color Color::White()
    {
        Color color;
        color.type = Color::Type::White;

        return color;
    }

    Color Color::BrightBlack()
    {
        Color color;
        color.type = Color::Type::BrightBlack;

        return color;
    }

    Color Color::BrightRed()
    {
        Color color;
        color.type = Color::Type::BrightRed;

        return color;
    }

    Color Color::BrightGreen()
    {
        Color color;
        color.type = Color::Type::BrightGreen;

        return color;
    }

    Color Color::BrightYellow()
    {
        Color color;
        color.type = Color::Type::BrightYellow;

        return color;
    }

    Color Color::BrightBlue()
    {
        Color color;
        color.type = Color::Type::BrightBlue;

        return color;
    }

    Color Color::BrightMagenta()
    {
        Color color;
        color.type = Color::Type::BrightMagenta;

        return color;
    }

    Color Color::BrightCyan()
    {
        Color color;
        color.type = Color::Type::BrightCyan;

        return color;
    }

    Color Color::BrightWhite()
    {
        Color color;
        color.type = Color::Type::BrightWhite;

        return color;
    }

    Color Color::Reset()
    {
        Color color;
        color.type = Color::Type::Reset;

        return color;
    }

    Color Color::RGB(uint32_t rgb)
    {
        Color color;
        color.type = Color::Type::RGB;
        color.rgb = rgb;

        return color;
    }

    Color Color::RGB(uint8_t r, uint8_t g, uint8_t b)
    {
        Color color;
        color.type = Color::Type::RGB;
        color.rgb = (r << 4) | (g << 2) | b;

        return color;
    }

    Color Color::Color8bit(uint8_t col)
    {
        Color color;
        color.type = Color::Type::Color8bit;
        color.color_8bit = col;

        return color;
    }

    bool Color::operator==(const Color& other) const
    {
        if (type != other.type)
            return false;
        
        if (type == Type::RGB)
            return rgb == other.rgb;
        
        if (type == Type::Color8bit)
            return color_8bit == other.color_8bit;
        
        return true;
    }

    bool Color::operator!=(const Color& other) const
    {
        return !(operator==(other));
    }
}