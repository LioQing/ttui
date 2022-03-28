#pragma once

#include <ttui/core/tcon.hpp>

namespace ttui { namespace cursor
{
    /**
     * @brief Set the text cursor to be visible or not.
     * 
     * @param visible Boolean indicating whether visible or not
     */
    void SetVisible(bool visible);

    /**
     * @brief Set the text cursor position. Note that Handle::Draw overwrite cursor position.
     * 
     * @param x The x position
     * @param y The y position
     */
    void SetPos(uint16_t x, uint16_t y);
}}