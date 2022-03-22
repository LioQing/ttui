#include <ttui/Border.hpp>

namespace ttui
{
    Border::Border() : slices() 
    {
    }

    Border Border::None()
    {
        Border border;
        return border;
    }

    Border Border::Single(const Appearance& appear)
    {
        Border border;

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

    Border Border::Double(const Appearance& appear)
    {
        Border border;

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

    Border Border::Thick(const Appearance& appear)
    {
        Border border;

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

    Border Border::Custom(const std::array<std::string, Count>& borders, const Appearance& appear)
    {
        Border border;

        for (auto i = 0; i < Count; ++i)
        {
            border.slices.at(i) = Slice{ borders.at(i), appear };
        }

        return border;
    }
    
    Border Border::Custom(const std::array<Slice, Count>& borders)
    {
        Border border;
        border.slices = borders;
        return border;
    }
}