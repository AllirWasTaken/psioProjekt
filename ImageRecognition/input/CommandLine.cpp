#include "CommandLine.h"

#include <cassert>
#include <iostream>

#include "Exceptions.h"

namespace
{

void displayAvailableCommands(const CommandLine::Commands &availableCommands)
{
    std::cout << "Available commands:" << std::endl;
    for (auto it = availableCommands.begin(); it != availableCommands.end(); ++it)
    {
        const auto &command = *it;
        std::cout << command.getName();
        if (it != std::prev(availableCommands.end())) [[likely]]
            std::cout << " ";
        else [[unlikely]]
            std::cout << std::endl;
    }
    std::cout << "Type \"help <command>\" for more info." << std::endl;
}

void displaySpecificCommandHelp(const Command &command)
{
    std::cout << "Command: " << command.getName() << std::endl;
    std::cout << command.getDescription() << std::endl;
}

void help(CommandLine::Commands &availableCommands, const Command::Args &args)
{
    if (args.empty())
    {
        displayAvailableCommands(availableCommands);
        return;
    }

    const auto &commandName = args.front();
    auto it = availableCommands.find(commandName);
    if (it == availableCommands.end())
        throw UnknownCommandError(commandName);

    displaySpecificCommandHelp(*it);
}

Command createHelpCommand(CommandLine::Commands &availableCommands)
{
    return {
        "help",
        "Lists available commands",
        std::bind(help, std::ref(availableCommands), std::placeholders::_1)};
}

} // namespace

CommandLine::CommandLine()
{
    addCommand(createHelpCommand(commands));
}

void CommandLine::addCommand(Command command)
{
    const auto [it, inserted] = commands.insert(std::move(command));
    assert(inserted);
}

void CommandLine::call(const std::string_view command, const Command::Args &args)
{
    auto it = commands.find(command);
    if (it == commands.end())
        throw UnknownCommandError(command);

    auto &cmd = *it;
    cmd(args);
}