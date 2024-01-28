#pragma once

#include <string_view>

#include <common/Exceptions.h>

class WindowCloseRequested : ImageRecognitionError {
public:
    WindowCloseRequested(const std::string_view windowName)
        : ImageRecognitionError("User requested window \"" + std::string { windowName } + "\" to close")
    {
    }
};