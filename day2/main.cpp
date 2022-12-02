#include <fmt/core.h>

#include <functional>
#include <numeric>
#include <ranges>
#include <string_view>

#include <cstdio>
#include <cstdlib>

auto get_points_p1(std::string_view const code) -> int
{
    if (code == "A X")
        return 1 + 3;
    else if (code == "A Y")
        return 2 + 6;
    else if (code == "A Z")
        return 3 + 0;
    else if (code == "B X")
        return 1 + 0;
    else if (code == "B Y")
        return 2 + 3;
    else if (code == "B Z")
        return 3 + 6;
    else if (code == "C X")
        return 1 + 6;
    else if (code == "C Y")
        return 2 + 0;
    else if (code == "C Z")
        return 3 + 3;

    return 0;
}

auto solve_p1(std::string_view const data) -> int
{
    auto results_view = data |
            std::views::split('\n') |
            std::views::transform([](auto const value)
            {
                return get_points_p1({value.begin(), value.end()});
            });

    return std::reduce(results_view.begin(), results_view.end());
}

auto get_points_p2(std::string_view const code) -> int
{
    if (code == "A X")
        return 3 + 0;
    else if (code == "A Y")
        return 1 + 3;
    else if (code == "A Z")
        return 2 + 6;
    else if (code == "B X")
        return 1 + 0;
    else if (code == "B Y")
        return 2 + 3;
    else if (code == "B Z")
        return 3 + 6;
    else if (code == "C X")
        return 2 + 0;
    else if (code == "C Y")
        return 3 + 3;
    else if (code == "C Z")
        return 1 + 6;

    return 0;
}

auto solve_p2(std::string_view data) -> int
{
    auto results_view = data |
            std::views::split('\n') |
            std::views::transform([](auto const value)
            {
                return get_points_p2({value.begin(), value.end()});
            });

    return std::reduce(results_view.begin(), results_view.end());
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
    char buffer[size + 1];

    std::fseek(file, 0, SEEK_SET);
    std::fread(buffer, sizeof(char), size, file);
    std::fclose(file);
    buffer[size] = '\0';

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
        if (std::string_view{argv[1]} == "p1")
            return solve_p1(read_file_content(argv[2]));
        else if (std::string_view{argv[1]} == "p2")
            return solve_p2(read_file_content(argv[2]));
        else
        {
            fmt::print(stderr,"Unknown first argument.");
            std::exit(1);
        }
    });

    fmt::print("{}", result);
}
