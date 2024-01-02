#pragma once

#include <stdexcept>

class InvalidSocketError : public std::runtime_error
{
public:
    InvalidSocketError() : std::runtime_error("Client unsafely disconnected") {}
};