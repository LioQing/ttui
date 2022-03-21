#include <ttui/tcon.hpp>

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

        int32_t MapEscSeqToInputCode(const char* esc_seq, int32_t size, uint8_t& code, bool& is_alt);
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
                uint8_t code;
                bool is_alt;
                int seq_len = MapEscSeqToInputCode(buf.c_str() + i, buf.size() - i, code, is_alt);
                
                if (seq_len == 0)
                    continue;

                if (seq_len == 1)
                {
                    Event event;
                    event.type = Event::Input;
                    event.input.code = InputEvent::Escape;
                    event.input.is_esc = false;
                    event.input.is_alt = false;
                    event_queue.emplace_back(event);

                    continue;
                }

                i += seq_len - 1;

                Event event;
                event.type = Event::Input;
                event.input.code = InputEvent::Escape;
                event.input.is_esc = !is_alt;
                event.input.is_alt = is_alt;
                event_queue.emplace_back(event);
            }
            else
            {
                Event event;
                event.type = Event::Input;
                event.input.code = (uint8_t)buf.at(i);
                event.input.is_esc = false;
                event.input.is_alt = false;
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

    void SetCursorPos(uint16_t x, uint16_t y)
    {
        printf("%c[%d;%df", ESC, y, x);
    }

    void SetColor4bit(uint8_t col, Target target)
    {
        if (col < 8)
            col += 30;
        else if (col < 16)
            col += 82;
        
        printf("%c[%dm", ESC, col + (int)target);
    }

    void SetColor8bit(uint8_t col, Target target)
    {
        printf("%c[%d;5;%dm", ESC, (int)target + 38, col);
    }

    void SetColor24bit(uint8_t r, uint8_t g, uint8_t b, Target target)
    {
        printf("%c[%d;2;%d;%d;%dm", ESC, (int)target + 38, r, g, b);
    }

    void SetColor24bit(uint32_t rgb, Target target)
    {
        printf("%c[%d;2;%d;%d;%dm", ESC, (int)target + 38,
            rgb / (uint32_t)0x00010000 % 0x100,
            rgb / (uint32_t)0x00000100 % 0x100,
            rgb / (uint32_t)0x00000001 % 0x100
        );
    }

    void ResetColor(Target target)
    {
        printf("%c[%dm", ESC, (int)target + 39);
    }

    void SetStyle(uint8_t style, bool enable)
    {
        if (style & Style::Bold        ) printf("%c[%dm", ESC, 1 + !enable * 21);
        if (style & Style::Dim         ) printf("%c[%dm", ESC, 2 + !enable * 20);
        if (style & Style::Italic      ) printf("%c[%dm", ESC, 3 + !enable * 20);
        if (style & Style::Underline   ) printf("%c[%dm", ESC, 4 + !enable * 20);
        if (style & Style::Blink       ) printf("%c[%dm", ESC, 5 + !enable * 20);
        if (style & Style::Inversed    ) printf("%c[%dm", ESC, 6 + !enable * 20);
        if (style & Style::Invisible   ) printf("%c[%dm", ESC, 7 + !enable * 20);
        if (style & Style::CrossedOut  ) printf("%c[%dm", ESC, 8 + !enable * 20);
    }

    void ClearScreen()
    {
        SetCursorPos(0, 0);
        printf("%c[J", 0x1b);
        fflush(stdout);
    }

    void HideCursor()
    {
        printf("%c[?25l", ESC);
    }

    void ShowCursor()
    {
        printf("%c[?25h", ESC);
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

        int32_t MapEscSeqToInputCode(const char* esc_seq, int32_t size, uint8_t& code, bool& is_alt)
        {
            if (size < 1 || esc_seq[0] != ESC)
                return 0;

            is_alt = false;
            
            if (size < 2)
                return 1;
            
            if (size > 2)
            {
                if (esc_seq[1] == '1' || esc_seq[1] == 'O')
                {
                    if      (esc_seq[2] == 'P') { code = InputEvent::F1; return 3; }
                    else if (esc_seq[2] == 'Q') { code = InputEvent::F2; return 3; }
                    else if (esc_seq[2] == 'R') { code = InputEvent::F3; return 3; }
                    else if (esc_seq[2] == 'S') { code = InputEvent::F4; return 3; }
                }
                else if (esc_seq[1] == '[')
                {
                    if (size > 3)
                    {
                        if (esc_seq[3] == '~')
                        {
                            if      (esc_seq[2] == '1')     { code = InputEvent::Home;      return 4; }
                            else if (esc_seq[2] == '2')     { code = InputEvent::Insert;    return 4; }
                            else if (esc_seq[2] == '3')     { code = InputEvent::Delete;    return 4; }
                            else if (esc_seq[2] == '4')     { code = InputEvent::End;       return 4; }
                            else if (esc_seq[2] == '5')     { code = InputEvent::PgUp;      return 4; }
                            else if (esc_seq[2] == '6')     { code = InputEvent::PgDown;    return 4; }
                            else if (esc_seq[2] == '7')     { code = InputEvent::Home;      return 4; }
                            else if (esc_seq[2] == '8')     { code = InputEvent::End;       return 4; }
                        }
                        
                        if (size > 4 && esc_seq[4] == '~')
                        {
                            if      (esc_seq[2] == '1' && esc_seq[3] == '0')    { code = InputEvent::F0;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '1')    { code = InputEvent::F1;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '2')    { code = InputEvent::F2;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '3')    { code = InputEvent::F3;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '4')    { code = InputEvent::F4;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '5')    { code = InputEvent::F5;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '7')    { code = InputEvent::F6;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '8')    { code = InputEvent::F7;        return 5; }
                            else if (esc_seq[2] == '1' && esc_seq[3] == '9')    { code = InputEvent::F8;        return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '0')    { code = InputEvent::F9;        return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '1')    { code = InputEvent::F10;       return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '3')    { code = InputEvent::F11;       return 5; }
                            else if (esc_seq[2] == '2' && esc_seq[3] == '4')    { code = InputEvent::F12;       return 5; }
                        }
                    }

                    if      (esc_seq[2] == 'A') { code = InputEvent::Up;    return 3; }
                    else if (esc_seq[2] == 'B') { code = InputEvent::Down;  return 3; }
                    else if (esc_seq[2] == 'C') { code = InputEvent::Right; return 3; }
                    else if (esc_seq[2] == 'D') { code = InputEvent::Left;  return 3; }
                    else if (esc_seq[2] == 'F') { code = InputEvent::End;   return 3; }
                    else if (esc_seq[2] == 'H') { code = InputEvent::Home;  return 3; }
                }
            }
            
            if (isprint(esc_seq[1]))
            {
                is_alt = true;
                code = esc_seq[1];
                return 2;
            }

            return 1;
        }
    }
}