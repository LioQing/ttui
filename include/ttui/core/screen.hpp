#pragma once

#include <ttui/core/Color.hpp>

namespace ttui { namespace screen
{
    /**
     * @brief Clear the screen, cursor position will be (0, 0) after this. Note using this with Handle::Draw every frame will cause flickering, use Empty widget instead.
     * 
     * @param color 
     */
    void Clear(const Color& color = Color::Reset());
}}