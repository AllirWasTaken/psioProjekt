#pragma once

#include <set>
#include <string_view>

#include "Command.h"

class CommandLine
{
public:
    using Commands = std::set<Command, std::less<>>;

    CommandLine();

    void addCommand(Command command);
    void call(const std::string_view command, const Command::Args& args);

private:
    Commands commands;
};