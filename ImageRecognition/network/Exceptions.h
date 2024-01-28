#pragma once

#include <common/Exceptions.h>

class NetworkError : ImageRecognitionError
{
public:
    using ImageRecognitionError::ImageRecognitionError;
};

class ConnectionFailedError : NetworkError
{
public:
    using NetworkError::NetworkError;
};