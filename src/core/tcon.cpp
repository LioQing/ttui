#include <ttui/core/tcon.hpp>

#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>

namespace tcon
{
    namespace
    {
        constexpr char ESC = 0x1b;

        enum Signal : sig_atomic_t
        {
            None    = 0,
            Exit    = 1 << 0,
            Resize  = 1 << 1,
        };

        volatile sig_atomic_t signal_status = Signal::None;
        void SAHandler(int32_t signal);

        bool GetInputBuf(std::string& buf);

        int32_t InputEventHandle(const char* esc_seq, int32_t size, Event& event);
    }

    bool Handle::initialized = false;

    bool Handle::Init()
    {
        if (initialized)
            return false;

        initialized = true;

        tcgetattr(STDIN_FILENO, &init_term);

        termios term = init_term;
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);

        struct sigaction sa;
        sa.sa_handler = &SAHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGWINCH, &sa, NULL) != 0)
            return false;

        if (sigaction(SIGTERM, &sa, NULL) != 0)
            return false;

        if (sigaction(SIGINT, &sa, NULL) != 0)
            return false;

        UpdateSize();

        return true;
    }

    Handle::~Handle()
    {
        End();
    }

    bool Handle::End()
    {
        if (!initialized)
            return true;

        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        if (sigaction(SIGWINCH, &sa, NULL) != 0)
            return false;

        if (sigaction(SIGTERM, &sa, NULL) != 0)
            return false;

        if (sigaction(SIGINT, &sa, NULL) != 0)
            return false;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &init_term) != 0)
            return false;

        initialized = false;

        return true;
    }

    bool Handle::UpdateSize()
    {
        struct winsize size;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) != 0)
            return false;
            
        width = size.ws_col;
        height = size.ws_row;

        return true;
    }

    uint16_t Handle::GetWidth() const
    {
        return width;
    }

    uint16_t Handle::GetHeight() const
    {
        return height;
    }
    
    void Handle::GetSize(uint16_t& x, uint16_t& y) const
    {
        x = width;
        y = height;
    }
    
    bool Handle::PollEvent(Event& event)
    {
        if (!event_queue.empty())
        {
            event = event_queue.front();
            event_queue.pop_front();

            return true;
        }

        // exit event
        if ((signal_status & Signal::Exit) != 0)
        {
            event_queue.emplace_back(Event{ Event::Exit });
        }

        // resize event
        if ((signal_status & Signal::Resize) != 0)
        {
            UpdateSize();
            Event event;
            event.type = Event::Resize;
            event.resize = ResizeEvent{ width, height };

            event_queue.emplace_back(event);
        }

        signal_status = Signal::None;

        // input event
        std::string buf;
        if (!GetInputBuf(buf) || buf.empty())
            return false;

        for (size_t i = 0; i < buf.size(); ++i)
        {
            if (buf.at(i) == ESC)
            {
                Event event;
                int seq_len = InputEventHandle(buf.c_str() + i, buf.size() - i, event);
                
                if (seq_len == 0)
                    continue;

                i += seq_len - 1;

                event_queue.emplace_back(event);
            }
            else
            {
                Event event;
                event.type = Event::Key;
                event.key.code = (uint8_t)buf.at(i);
                event.key.is_esc = false;
                event.key.is_alt = false;
                event_queue.emplace_back(event);
            }
        }

        // pop event
        if (!event_queue.empty())
        {
            event = event_queue.front();
            event_queue.pop_front();

            return true;
        }

        return false;
    }

    bool Handle::IsInitialized()
    {
        return Handle::initialized;
    }

    std::string SetCursorPos(uint16_t x, uint16_t y)
    {
        return std::string(1, ESC) + "[" + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "f";
    }

    std::string SetAppearance(const std::vector<std::string>& vec)
    {
        std::string str = "\x1b[";

        for (const auto& seq : vec)
        {
            if (!seq.empty())
                str += seq + ";";
        }

        str.resize(str.size() - 1);

        return str + "m";
    }

    std::string Color4bit(uint8_t col, Target target)
    {
        if (col < 8)
            col += 30;
        else if (col < 16)
            col += 82;
        
        return std::to_string(col + (int)target);
    }

    std::string Color8bit(uint8_t col, Target target)
    {
        return std::to_string((int)target + 38) + ";5;" + std::to_string(col);
    }

    std::string Color24bit(uint8_t r, uint8_t g, uint8_t b, Target target)
    {
        return std::to_string((int)target + 38) + ";2;"
            + std::to_string(r) + ";"
            + std::to_string(g) + ";"
            + std::to_string(b);
    }

    std::string Color24bit(uint32_t rgb, Target target)
    {
        return std::to_string((int)target + 38) + ";2;"
            + std::to_string(rgb / (uint32_t)0x00010000 % 0x100) + ";"
            + std::to_string(rgb / (uint32_t)0x00000100 % 0x100) + ";"
            + std::to_string(rgb / (uint32_t)0x00000001 % 0x100);
    }

    std::string ColorReset(Target target)
    {
        return std::to_string((int)target + 39);
    }

    std::string Style(uint8_t style, bool enable)
    {
        std::string str;
        if (style & Style::Bold        ) str += std::to_string(1 + !enable * 21) + ";";
        if (style & Style::Dim         ) str += std::to_string(2 + !enable * 20) + ";";
        if (style & Style::Italic      ) str += std::to_string(3 + !enable * 20) + ";";
        if (style & Style::Underline   ) str += std::to_string(4 + !enable * 20) + ";";
        if (style & Style::Blink       ) str += std::to_string(5 + !enable * 20) + ";";
        if (style & Style::Inversed    ) str += std::to_string(6 + !enable * 20) + ";";
        if (style & Style::Invisible   ) str += std::to_string(7 + !enable * 20) + ";";
        if (style & Style::CrossedOut  ) str += std::to_string(8 + !enable * 20) + ";";

        if (str.empty())
            return "";
        return str.substr(0, str.size() - 1);
    }

    std::string SetClearScreen()
    {
        return SetCursorPos(0, 0) + std::string(1, ESC) + "[J";
    }

    std::string SetHideCursor()
    {
        return std::string(1, ESC) + "[?25l";
    }

    std::string SetShowCursor()
    {
        return std::string(1, ESC) + "[?25h";
    }

    std::string SetEnableMouseTracking()
    {
        return std::string(1, ESC) + "[?1000;1006;1015h";
    }

    std::string SetDisableMouseTracking()
    {
        return std::string(1, ESC) + "[?1000;1006;1015l";
    }

    namespace
    {
        void SAHandler(int32_t signal)
        {
            switch (signal)
            {
                case SIGWINCH:
                    signal_status |= Signal::Resize;
                    return;

                case SIGTERM:
                case SIGINT:
                    signal_status |= Signal::Exit;
                    return;

                default: return;
            }
        }
        
        bool GetInputBuf(std::string& buf)
        {
            termios term;
            tcflag_t prev_lflag;

            tcgetattr(STDIN_FILENO, &term);
            prev_lflag = term.c_lflag;

            term.c_lflag &= ~(ICANON | ECHO);
            term.c_cc[VTIME] = 0;
            term.c_cc[VMIN] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &term);

            // number of bytes to be read
            int bytes_read;
            if (ioctl(STDIN_FILENO, FIONREAD, &bytes_read) != 0)
                return false;

            // read the inputs
            char* c_buf = (char*)malloc((bytes_read) * sizeof(char));
            if (read(STDIN_FILENO, c_buf, bytes_read) < 0)
                return false;
            
            buf = std::string(c_buf, bytes_read);

            // restore terminal attributes
            term.c_lflag = prev_lflag;
            tcsetattr(STDIN_FILENO, TCSANOW, &term);

            return true;
        }

        int32_t InputEventHandle(const char* esc_seq, int32_t size, Event& event)
        {
            if (size < 1 || esc_seq[0] != ESC)
                return 0;

            event.type = Event::Key;

            if (size < 2)
            {
                event.key.code = KeyEvent::Escape;
                event.key.is_esc = false;
                event.key.is_alt = false;
                return 1;
            }
            
            event.key.is_alt = false;
            event.key.is_esc = true;

            if (size > 2)
            {
                if (esc_seq[1] == '1' || esc_seq[1] == 'O')
                {
                    if      (esc_seq[2] == 'P') { event.key.code = KeyEvent::F1; return 3; }
                    else if (esc_seq[2] == 'Q') { event.key.code = KeyEvent::F2; return 3; }
                    else if (esc_seq[2] == 'R') { event.key.code = KeyEvent::F3; return 3; }
                    else if (esc_seq[2] == 'S') { event.key.code = KeyEvent::F4; return 3; }
                }
                else if (esc_seq[1] == '[')
                {
                    // mouse event
                    {
                        int x, y;
                        int action;
                        char postfix;
                        int char_read;
                        if (sscanf(esc_seq + 2, "<%d;%d;%d%[Mm]%n", &action, &x, &y, &postfix, &char_read) == 4)
                        {
                            if (action == 0 || action == 1 || action == 2)
                            {
                                event.type = Event::MouseButton;
                                event.mouse_button.x = x;
                                event.mouse_button.y = y;
                                event.mouse_button.is_down = postfix == 'M';

                                if      (action == 0) { event.mouse_button.button = MouseButtonEvent::Left;   return char_read + 2; }
                                else if (action == 1) { event.mouse_button.button = MouseButtonEvent::Middle; return char_read + 2; }
                                else if (action == 2) { event.mouse_button.button = MouseButtonEvent::Right;  return char_read + 2; }
                            }
                            else if (action == 64 || action == 65)
                            {
                                event.type = Event::MouseWheel;
                                event.mouse_wheel.x = x;
                                event.mouse_wheel.y = y;
                                event.mouse_wheel.delta = action == 64 ? 1 : -1;
                                return char_read + 2;
                            }
                        }
                    }

                    if (size > 3)
                    {
                        if (esc_seq[3] == '~')
                        {
                            if      (esc_seq[2] == '1')     { event.key.code = KeyEvent::Home;      return 4; }
                            else if (esc_seq[2] == '2')     { event.key.code = KeyEvent::Insert;    return 4; }
                            else if (esc_seq[2] == '3')     { event.key.code = KeyEvent::Delete;    return 4; }
                            else if (esc_seq[2] == '4')     { event.key.code = KeyEvent::End;       return 4; }
                            else if (esc_seq[2] == '5')     { event.key.code = KeyEvent::PgUp;      return 4; }
                            else if (esc_seq[2] == '6')     { event.key.code = KeyEvent::PgDown;    return 4; }
                            else if (esc_seq[2] == '7')     { event.key.code = KeyEvent::Home;      return 4; }
                            else if (esc_seq[2] == '8')     { event.key.code = KeyEvent::End;       return 4; }
                        }
                        
                        if (size > 4 && esc_seq[4] == '~')
                        {
                            if      (esc_seq[2] == '1' && esc_seq[3] == '0') { event.key.code = KeyEvent::F0;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '1') { event.key.code = KeyEvent::F1;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '2') { event.key.code = KeyEvent::F2;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '3') { event.key.code = KeyEvent::F3;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '4') { event.key.code = KeyEvent::F4;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '5') { event.key.code = KeyEvent::F5;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '7') { event.key.code = KeyEvent::F6;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '8') { event.key.code = KeyEvent::F7;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '9') { event.key.code = KeyEvent::F8;        return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '0') { event.key.code = KeyEvent::F9;        return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '1') { event.key.code = KeyEvent::F10;       return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '3') { event.key.code = KeyEvent::F11;       return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '4') { event.key.code = KeyEvent::F12;       return 5; }
                        }
                    }

                    if      (esc_seq[2] == 'A') { event.key.code = KeyEvent::Up;    return 3; }
                    else if (esc_seq[2] == 'B') { event.key.code = KeyEvent::Down;  return 3; }
                    else if (esc_seq[2] == 'C') { event.key.code = KeyEvent::Right; return 3; }
                    else if (esc_seq[2] == 'D') { event.key.code = KeyEvent::Left;  return 3; }
                    else if (esc_seq[2] == 'F') { event.key.code = KeyEvent::End;   return 3; }
                    else if (esc_seq[2] == 'H') { event.key.code = KeyEvent::Home;  return 3; }
                }
            }
            
            if (isprint(esc_seq[1]))
            {
                event.key.is_alt = true;
                event.key.is_esc = false;
                event.key.code = esc_seq[1];
                return 2;
            }

            event.key.code = KeyEvent::Escape;
            event.key.is_esc = false;
            event.key.is_alt = false;
            return 1;
        }
    }
}