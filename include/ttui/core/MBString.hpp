#pragma once

#include <string>
#include <stdexcept>

namespace ttui { namespace MBString 
{
    /**
     * @brief Get the size of a multibyte string. (from cppreference.com)
     * 
     * @param s The multibyte string
     * @return size_t The size of the multibyte string
     */
    size_t Size(const std::string& s);

    /**
     * @brief Get the substring of a multibyte string.
     * 
     * @param s The multibyte string
     * @param pos The start position of the substring
     * @param n The length of the substring
     * @return size_t The substring
     */
    std::string Substr(const std::string& s, size_t pos, size_t n);
}}