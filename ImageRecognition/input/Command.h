#pragma once

#include <functional>
#include <string>
#include <string_view>

class Command {
public:
    using Args = std::vector<std::string>;
    using Func = std::function<void(const Args&)>;

    Command(std::string name, std::string description, Func func);

    void operator()(const Args& args) const;

    std::string getName() const;
    std::string getDescription() const;

private:
    friend bool operator==(const Command& lhs, const Command& rhs);
    friend bool operator==(const Command& command, const std::string_view commandName);
    friend bool operator==(const std::string_view commandName, const Command& command);

    friend std::weak_ordering operator<=>(const Command& lhs, const Command& rhs);
    friend std::weak_ordering operator<=>(const Command& command, const std::string_view commandName);
    friend std::weak_ordering operator<=>(const std::string_view commandName, const Command& command);

    std::string name;
    std::string description;
    Func func;
};