#include "ImageRecognition.h"

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "common/SystemType.h"

#include "common/async/ThreadPool.h"

#include "input/CommandLine.h"
#include "input/Exceptions.h"

#include "ImageDetection.h"

namespace {

Command createFpsCommand()
{
    return { "fps", "", [](const Command::Args&) {} };
}

Command createObjectsCommand()
{
    return { "objects", "", [](const Command::Args&) {} };
}

Command createStatsCommand()
{
    return { "stats", "", [](const Command::Args&) {} };
}

Command createSettingsCommand()
{
    return { "settings", "", [](const Command::Args&) {} };
}

Command createCalibrationCommand()
{
    return { "calibration", "", [](const Command::Args&) {} };
}

Command createDebugCommand()
{
    return { "debug", "", [](const Command::Args&) {} };
}

Command createSampleCommand()
{
    return { "sample", "", [](const Command::Args&) {} };
}

Command createClearCommand()
{
    return { "clear", "", [](const Command::Args&) {} };
}

Command createExitCommand(std::stop_source& stopSource)
{
    return { "exit", "", [&](const Command::Args&) { stopSource.request_stop(); } };
}

std::pair<std::string, Command::Args> parseInput(const std::string_view input)
{
    if (input.empty())
        throw EmptyInputError();

    std::stringstream ss;
    if (!(ss << input))
        throw UserInputError("Failed to read command");

    std::string command;
    ss >> command;

    Command::Args args;
    std::string arg;
    while (ss >> std::quoted(arg)) {
        args.push_back(arg);
    }
    return { std::move(command), std::move(args) };
}

} // namespace

class ImageRecognition::Impl {
public:
    Impl()
    {
        commandLine.addCommand(createFpsCommand());
        commandLine.addCommand(createObjectsCommand());
        commandLine.addCommand(createStatsCommand());
        commandLine.addCommand(createSettingsCommand());
        commandLine.addCommand(createCalibrationCommand());
        commandLine.addCommand(createDebugCommand());
        commandLine.addCommand(createSampleCommand());
        commandLine.addCommand(createClearCommand());
        commandLine.addCommand(createExitCommand(stopSource));
    }

    int run(int /*argc*/, char** /*argv*/)
    try
    {
        threadPool.start();
        ImageDetection imageDetection{threadPool};
        threadPool.schedule([this](std::stop_token stopToken) mutable {
            inputLoop(stopToken);
        });
        return imageDetection.run(stopSource.get_token());
    }
    catch(const std::exception& e)
    {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "Unknown critical error" << std::endl;
        return -2;
    }

private:
    void inputLoop(std::stop_token threadStopToken) noexcept
    try
    {
        struct Finally
        {
            ~Finally() {
                stopSource.request_stop();
            }
            std::stop_source& stopSource;
        } finally{stopSource};


        auto stopToken = stopSource.get_token();
        while (!threadStopToken.stop_requested() && !stopToken.stop_requested()) {
            try {
                auto input = getUserInput();
                clearScreen();
                handleUserInput(std::move(input));
            } catch (const UserInputError& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "InputThread: Critical error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "InputThread: Unknown critical error" << std::endl;
    }

    std::string getUserInput() const
    {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    void clearScreen() const
    {
        if constexpr(getSystemType() == SystemType::Windows)
            std::system("cls");
        else if constexpr(getSystemType() == SystemType::Unix)
            std::system("clear");
    }

    void handleUserInput(const std::string_view input)
    {
        auto [command, args] = parseInput(input);
        commandLine.call(command, args);
    }

    ThreadPool threadPool;
    std::stop_source stopSource;
    CommandLine commandLine;
};

ImageRecognition::ImageRecognition()
    : impl(std::make_unique<Impl>())
{
}

ImageRecognition::~ImageRecognition() = default;

int ImageRecognition::run(int argc, char** argv)
{
    return impl->run(argc, argv);
}