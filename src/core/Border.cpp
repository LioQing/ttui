#include <ttui/core/Border.hpp>

namespace ttui
{
    Border::Border() : is_none(true), slices() 
    {
    }

    Border Border::None()
    {
        Border border;
        border.is_none = true;

        return border;
    }

    Border Border::Single(const std::string& title, const Appearance& appear)
    {
        Border border;
        border.is_none = false;
        border.title = title;

        border.slices.at(Left)        = Slice{ "\u2502", appear };
        border.slices.at(Right)       = Slice{ "\u2502", appear };
        border.slices.at(Top)         = Slice{ "\u2500", appear };
        border.slices.at(Bottom)      = Slice{ "\u2500", appear };
        border.slices.at(TopLeft)     = Slice{ "\u250c", appear };
        border.slices.at(TopRight)    = Slice{ "\u2510", appear };
        border.slices.at(BottomLeft)  = Slice{ "\u2514", appear };
        border.slices.at(BottomRight) = Slice{ "\u2518", appear };

        return border;
    }

    Border Border::Double(const std::string& title, const Appearance& appear)
    {
        Border border;
        border.is_none = false;
        border.title = title;

        border.slices.at(Left)        = Slice{ "\u2551", appear };
        border.slices.at(Right)       = Slice{ "\u2551", appear };
        border.slices.at(Top)         = Slice{ "\u2550", appear };
        border.slices.at(Bottom)      = Slice{ "\u2550", appear };
        border.slices.at(TopLeft)     = Slice{ "\u2554", appear };
        border.slices.at(TopRight)    = Slice{ "\u2557", appear };
        border.slices.at(BottomLeft)  = Slice{ "\u255a", appear };
        border.slices.at(BottomRight) = Slice{ "\u255d", appear };

        return border;
    }

    Border Border::Thick(const std::string& title, const Appearance& appear)
    {
        Border border;
        border.is_none = false;
        border.title = title;

        border.slices.at(Left)        = Slice{ "\u2503", appear };
        border.slices.at(Right)       = Slice{ "\u2503", appear };
        border.slices.at(Top)         = Slice{ "\u2501", appear };
        border.slices.at(Bottom)      = Slice{ "\u2501", appear };
        border.slices.at(TopLeft)     = Slice{ "\u250f", appear };
        border.slices.at(TopRight)    = Slice{ "\u2513", appear };
        border.slices.at(BottomLeft)  = Slice{ "\u2517", appear };
        border.slices.at(BottomRight) = Slice{ "\u251b", appear };

        return border;
    }

    Border Border::Custom(const std::array<std::string, Count>& borders, const std::string& title, const Appearance& appear)
    {
        Border border;
        border.is_none = false;
        border.title = title;

        for (auto i = 0; i < Count; ++i)
        {
            border.slices.at(i) = Slice{ borders.at(i), appear };
        }

        return border;
    }
    
    Border Border::Custom(const std::array<Slice, Count>& borders, const std::string& title)
    {
        Border border;
        border.is_none = false;
        border.slices = borders;
        border.title = title;

        return border;
    }
}