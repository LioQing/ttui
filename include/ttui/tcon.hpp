#pragma once

#include <cstdint>
#include <string>
#include <signal.h>
#include <deque>
#include <termios.h>

namespace tcon
{
    /**
     * @brief A structure containing input event information.
     * 
     */
    struct InputEvent
    {
        enum : uint8_t
        {
            // escape codes
            F0,
            F1, F2, F3, F4,
            F5, F6, F7, F8,
            F9, F10, F11, F12,
            Up, Down, Right, Left,
            Home, Insert, Delete, End, PgUp, PgDown,

            // normal codes (named)
            Enter = 13,
            Escape = 27,
            Tab = 9,
        };

        uint8_t code;
        bool is_esc;
        bool is_alt;
    };

    /**
     * @brief A structure containing resize event information.
     * 
     */
    struct ResizeEvent
    {
        uint16_t width;
        uint16_t height;
    };

    /**
     * @brief A union of all the event structures.
     * 
     */
    struct Event
    {
        enum Type
        {
            Exit, Resize, Input,
        };

        Type type;

        union
        {
            InputEvent input;
            ResizeEvent resize;
        };
    };

    /**
     * @brief Enum for specifying what target to set color (foreground or background).
     * 
     */
    enum class Target
    {
        Foreground, Background = 10,
    };

    /**
     * @brief Enum for specifying the colors.
     * 
     */
    struct Color
    {
        enum : uint8_t
        {
            Black = 0,
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
        };
    };

    /**
     * @brief Enum for specifying style of the text.
     * 
     */
    struct Style
    {
        enum : uint8_t
        {
            Bold            = 0x1,
            Dim             = 0x2,
            Italic          = 0x4,
            Underline       = 0x8,
            Blink           = 0x10,
            Inversed        = 0x20,
            Invisible       = 0x40,
            CrossedOut      = 0x80,

            All = Bold | Dim | Italic | Underline | Blink | 
                Inversed | Invisible | CrossedOut,
        };
    };

    struct Handle
    {
        Handle() = default;
        ~Handle();

        /**
         * @brief Initialize tcon in this Handle.
         * 
         * @return bool Boolean indicating success or not
         */
        bool Init();

        /**
         * @brief Restore default terminal and dettach signal handler.
         * 
         * @return bool Boolean indicating success or not
         */
        bool End();

        /**
         * @brief Update the terminal size.
         * 
         * @return bool Boolean indicating success or not
         */
        bool UpdateSize();

        /**
         * @brief Get the width of terminal.
         * 
         * @return uint16_t The width
         */
        uint16_t GetWidth() const;

        /**
         * @brief Get the height of terminal.
         * 
         * @return uint16_t The height
         */
        uint16_t GetHeight() const;

        /**
         * @brief Get the size of terminal.
         * 
         * @param x The width value to be returned
         * @param y The height value to be returned
         */
        void GetSize(uint16_t& x, uint16_t& y) const;

        /**
         * @brief Poll events.
         * 
         * @param event The Event struct to be returned
         * @return bool Boolean indicating whether an event is polled
         */
        bool PollEvent(Event& event);

        /**
         * @brief Check is there any Handle initialized.
         * 
         * @return bool Boolean indicating whether initialized or not
         */
        static bool IsInitialized();

    private:

        Handle(const Handle&) = default;
    
        static bool initialized;
        termios init_term;

        uint16_t width;
        uint16_t height;

        std::deque<Event> event_queue;
    };

    /**
     * @brief Set the cursor position.
     * 
     * @param x The x coordinate
     * @param y The y coordinate
     */
    void SetCursorPos(uint16_t x, uint16_t y);
    
    /**
     * @brief Set the color in 4 bit format (16 colors).
     * 
     * @param col The color to be set
     * @param target The target (foreground or background)
     */
    void SetColor4bit(uint8_t col, Target target);

    /**
     * @brief Set the color in 8 bit format (256 colors).
     * 
     * @param col The color to be set
     * @param target The target (foreground or background)
     */
    void SetColor8bit(uint8_t col, Target target);

    /**
     * @brief Set the color in 24 bit format (rgb color).
     * 
     * @param r Red
     * @param g Breen
     * @param b Blue
     * @param target The target (foreground or background)
     */
    void SetColor24bit(uint8_t r, uint8_t g, uint8_t b, Target target);

    /**
     * @brief Set the color in 24 bit format (rgb color).
     * 
     * @param rgb The color to be set
     * @param target The target (foreground or background)
     */
    void SetColor24bit(uint32_t rgb, Target target);

    /**
     * @brief Reset the color to default.
     * 
     * @param target The target (foreground or background)
     */
    void ResetColor(Target target);

    /**
     * @brief Set the style of text.
     * 
     * @param style The style to be set
     * @param enable Whether enable to disable the style
     */
    void SetStyle(uint8_t style, bool enable);

    /**
     * @brief Clear the screen, cursor position will be (0, 0) after this.
     * 
     */
    void ClearScreen();

    /**
     * @brief Hide the cursor.
     * 
     */
    void HideCursor();

    /**
     * @brief Show the cursor.
     * 
     */
    void ShowCursor();
}