#include <fmt/core.h>

#include <charconv>
#include <functional>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

#include <cstdio>
#include <cstdlib>

auto solve_p1(std::string_view data) -> int
{
    auto current_value = 0;
    auto max_value = 0;

    for (auto const line : data | std::views::split('\n'))
    {
        if (int value; std::from_chars(line.begin(), line.end(), value).ec == std::errc{})
            current_value += value;
        else
        {
            max_value = std::max(current_value, max_value);
            current_value = 0;
        }
    }

    return max_value;
}

auto solve_p2(std::string_view data) -> int
{
    auto current_value = 0;
    std::vector<int> max_values = {0, 0, 0};

    for (auto const line : data | std::views::split('\n'))
    {
        if (int value; std::from_chars(line.begin(), line.end(), value).ec == std::errc{})
            current_value += value;
        else
        {
            max_values.push_back(current_value);
            std::ranges::sort(max_values, std::ranges::greater());
            max_values.pop_back();
            current_value = 0;
        }
    }

    return std::reduce(max_values.begin(), max_values.end());
}

auto read_file_content(char const * const filename) -> std::string
{
    auto* const file = std::fopen(filename, "r");

    if (file == nullptr)
    {
        fmt::print(stderr,"Error reading file: {}", filename);
        std::exit(1);
    }

    std::fseek(file, 0, SEEK_END);
    auto const size = std::ftell(file);
    char buffer[size];

    std::fseek(file, 0, SEEK_SET);
    std::fread(buffer, sizeof(char), size, file);
    std::fclose(file);

    return buffer;
}

auto main(int argc, char* argv[]) -> int
{
    if (argc != 3)
    {
        fmt::print(stderr,"Wrong number of arguments.");
        std::exit(1);
    }

    auto const result = std::invoke([argv]() {
        if (std::string_view (argv[1]) == "p1")
            return solve_p1(read_file_content(argv[2]));
        else if (std::string_view(argv[1]) == "p2")
            return solve_p2(read_file_content(argv[2]));
        else
        {
            fmt::print(stderr,"Unknown first argument.");
            std::exit(1);
        }
    });

    fmt::print("{}", result);
}
