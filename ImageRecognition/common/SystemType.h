#pragma once

enum class SystemType
{
    Windows,
    Unix
};

constexpr SystemType getSystemType() noexcept
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    return SystemType::Windows;
#else
    return SystemType::Unix;
#endif
}