#pragma once

#include <common/Exceptions.h>

class UserInputError : public ImageRecognitionError
{
public:
    using ImageRecognitionError::ImageRecognitionError;

};

class EmptyInputError : public UserInputError
{
public:
    EmptyInputError()
        : UserInputError(
              "Command can't be empty. Type \"help\" for list of available commands.") {}
};

class UnknownCommandError : public UserInputError
{
public:
    UnknownCommandError(const std::string_view command)
        : UserInputError(
            "Unknown command: " + std::string { command } + ": Type \"help\" for list of available commands.")
    {
    }
};