#include "Command.h"

#include <cassert>

Command::Command(std::string name, std::string description, Func func)
    : name(name)
    , description(description)
    , func(func)
{
    assert(func != nullptr);
}

void Command::operator()(const Args& args) const
{
    func(args);
}

std::string Command::getName() const
{
    return name;
}

std::string Command::getDescription() const
{
    return description;
}

bool operator==(const Command& lhs, const Command& rhs)
{
    return lhs.name == rhs.name;
}

bool operator==(const Command& command, const std::string_view commandName)
{
    return command.name == commandName;
}

bool operator==(const std::string_view commandName, const Command& command)
{
    return commandName == command.name;
}

std::weak_ordering operator<=>(const Command& lhs, const Command& rhs)
{
    return lhs.name <=> rhs.name;
}

std::weak_ordering operator<=>(const Command& command, const std::string_view commandName)
{
    return command.name <=> commandName;
}

std::weak_ordering operator<=>(const std::string_view commandName, const Command& command)
{
    return commandName <=> command.name;
}