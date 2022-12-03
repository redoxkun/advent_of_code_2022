#include <fmt/core.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>
#include <string_view>

#include <cstdio>
#include <cstdlib>

auto common_item(std::string first_rucksack, std::string second_rucksack) -> std::string
{
    std::ranges::sort(first_rucksack);
    std::ranges::sort(second_rucksack);

    std::string result;
    std::ranges::set_intersection(first_rucksack, second_rucksack, std::back_inserter(result));

    return result;
}

auto get_item_priority(char const item) -> int
{
    return (item < 'a') ? item - 'A' + 27 : item - 'a' + 1;
}

auto solve_p1(std::string_view const data) -> int
{
    auto result_view = data |
            std::views::split('\n') |
            std::views::transform([](auto const line)
            {
                std::string_view rucksacks_content{line.begin(), line.end()};
                return common_item(std::string{rucksacks_content.substr(0, rucksacks_content.size() / 2)},
                                                     std::string{rucksacks_content.substr(rucksacks_content.size() / 2)})[0];
            }) |
            std::views::transform(get_item_priority);

    return std::reduce(result_view.begin(), result_view.end());
}

auto solve_p2(std::string_view const data) -> int
{
    std::vector<std::vector<std::string>> elves_groups;
    std::vector<std::string> current_group;

    for (auto const line : data | std::views::split('\n'))
    {
        current_group.emplace_back(line.begin(), line.end());
        if (current_group.size() == 3)
        {
            elves_groups.push_back(current_group);
            current_group.clear();
        }
    }

    auto badges_value_view = elves_groups |
            std::views::transform([](auto const& elves)
            {
                return std::reduce(elves.begin(), elves.end(), elves[0], common_item)[0];
            }) |
            std::views::transform(get_item_priority);

    return std::reduce(badges_value_view.begin(), badges_value_view.end());
}

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
