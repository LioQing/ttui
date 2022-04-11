#include <ttui/core/MBString.hpp>

namespace ttui { namespace MBString 
{
    size_t Size(const std::string& s)
    {
        size_t result = 0;
        const char* ptr = s.data();
        const char* end = ptr + s.size();

        mblen(nullptr, 0);
        while (ptr < end)
        {
            int next = mblen(ptr, end - ptr);
            if (next == -1)
            {
                throw std::runtime_error("MBString::Size(): conversion error");
            }
            ptr += next;
            ++result;
        }
        return result;
    }

    std::string Substr(const std::string& s, size_t pos, size_t n)
    {
        const char* ptr = s.data();
        const char* end = ptr + s.size();
        size_t curr_pos = 0;
        size_t start_pos = 0;
        size_t end_pos = 0;

        mblen(nullptr, 0);
        while (ptr < end)
        {
            int next = mblen(ptr, end - ptr);
            if (next == -1)
            {
                throw std::runtime_error("MBString::Substr(): conversion error");
            }

            if (curr_pos == pos)
            {
                start_pos = ptr - s.data();
            }
            else if (curr_pos == pos + n)
            {
                end_pos = ptr - s.data();
                break;
            }
            
            ptr += next;
            ++curr_pos;
        }

        if (curr_pos == pos + n)
        {
            end_pos = ptr - s.data();
        }

        return s.substr(start_pos, end_pos - start_pos);
    }
}}