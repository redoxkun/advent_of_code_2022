#include <fmt/core.h>

#include <functional>

#include <cstdio>
#include <cstdlib>

auto solve_p1(std::string_view data) -> int;
auto solve_p2(std::string_view data) -> int;

auto read_file_content(char const * const filename) -> std::string
{
    auto * const file = std::fopen(filename, "r");

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

    if (buffer[size - 1] == '\n')
        buffer[size - 1] = '\0';

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
