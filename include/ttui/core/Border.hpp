#pragma once

#include <array>
#include <string>

#include <ttui/core/Appearance.hpp>

namespace ttui
{
    struct Border
    {
        struct Slice
        {
            std::string str;
            Appearance appear;
        };

        enum : uint8_t
        {
            Left,
            Right,
            Top,
            Bottom,
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight,

            Count
        };

        bool is_none;
        std::array<Slice, Count> slices;
        std::string title;

        Border();
        Border(const Border&) = default;

        static Border None();
        static Border Single(const std::string& title = "", const Appearance& appear = Appearance());
        static Border Double(const std::string& title = "", const Appearance& appear = Appearance());
        static Border Thick(const std::string& title = "", const Appearance& appear = Appearance());
        static Border Custom(const std::array<std::string, Count>& borders, const std::string& title = "", const Appearance& appear = Appearance());
        static Border Custom(const std::array<Slice, Count>& borders, const std::string& title = "");
    };
}