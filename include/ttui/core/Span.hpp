#pragma once

#include <string>

#include <ttui/core/Appearance.hpp>

namespace ttui
{
    struct Span
    {
        Span() = default;
        Span(const Span&) = default;

        Span(const std::string& str, const Appearance& appear = Appearance());

        std::string str;
        Appearance appear;
    };
}