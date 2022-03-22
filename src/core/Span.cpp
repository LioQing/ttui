#include <ttui/core/Span.hpp>

namespace ttui
{
    Span::Span(const std::string& str, const Appearance& appear)
        : str(str), appear(appear)
    {
    }
}