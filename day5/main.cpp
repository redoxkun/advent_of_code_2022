#include <fmt/core.h>

#include <algorithm>
#include <charconv>
#include <ranges>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <cstdio>
#include <cstdlib>

auto get_stack_positions(std::string_view const line) -> std::vector<std::pair<char, int>>
{
    std::vector<std::pair<char, int>> positions;

    for (std::size_t i = 0; i < line.size(); ++i)
    {
        if (line[i] != ' ')
            positions.emplace_back(line[i], i);
    }

    return positions;
}

template<std::ranges::view ViewType>
auto get_stacks_data(ViewType const data) -> std::unordered_map<char, std::vector<char>>
{
    std::vector<std::string_view> lines;
    std::ranges::transform(data | std::views::split('\n'), std::back_inserter(lines),
                           [](auto const line) { return std::string_view{line.begin(), line.end()}; });

    std::vector<std::pair<char, int>> positions;
    std::unordered_map<char, std::vector<char>> result;
    for (auto const line : lines | std::views::reverse)
    {
        if (positions.empty())
            positions = get_stack_positions(line);
        else
        {
            for (auto const[stack, position] : positions)
            {
                if (position < line.size() and line[position] != ' ')
                    result[stack].push_back(line[position]);
            }
        }
    }

    return result;
}

template<std::ranges::view ViewType>
auto get_moves_data(ViewType const data) -> std::vector<std::tuple<int, char, char>>
{
    std::vector<std::tuple<int, char, char>> moves;

    std::ranges::transform(data | std::views::split('\n'), std::back_inserter(moves),
    [](auto const line_view)
    {
        std::string_view line{line_view.begin(), line_view.end()};
        auto const f_position = line.find(" f");
        auto const t_position = line.find(" t");

        int crates;

        if (std::from_chars(line.begin() + 5, line.begin() + f_position, crates).ec != std::errc{})
        {
            fmt::print(stderr, "Unknown number of moves.");
            std::exit(1);
        }

        return std::tuple{crates, line[t_position - 1], line[t_position + 4]};
    });

    return moves;
}

void move_crates_cratemover9000(std::unordered_map<char, std::vector<char>>& stacks,
                                std::vector<std::tuple<int, char, char>> const& moves)
{
    for (auto const[crates, origin, destiny] : moves)
    {
        for (auto i = 0; i < crates; ++i)
        {
            stacks[destiny].push_back(stacks[origin].back());
            stacks[origin].pop_back();
        }
    }
}

void move_crates_cratemover9001(std::unordered_map<char, std::vector<char>>& stacks,
                                std::vector<std::tuple<int, char, char>> const& moves)
{
    for (auto const[crates, origin, destiny] : moves)
    {
        stacks[destiny].insert(stacks[destiny].end(),
                               stacks[origin].end() - crates, stacks[origin].end());

        stacks[origin].erase(stacks[origin].end() - crates, stacks[origin].end());
    }
}

auto get_result(std::unordered_map<char, std::vector<char>> const& stacks) -> std::string
{
    std::vector<char> stack_names;
    std::ranges::transform(stacks, std::back_inserter(stack_names),
                           [](auto const& pair) { return pair.first; });

    std::ranges::sort(stack_names);

    std::string result;
    std::ranges::transform(stack_names, std::back_inserter(result),
                           [&stacks](char const stack) { return stacks.at(stack).back(); });

    return result;
}

auto solve_p1(std::string_view const data)
{
    auto data_view = data |
            std::views::split(std::string_view{"\n\n"});

    auto data_iterator = data_view.begin();
    auto stacks_data = get_stacks_data(*(data_iterator++));
    auto const moves_data = get_moves_data(*data_iterator);

    move_crates_cratemover9000(stacks_data, moves_data);

    return get_result(stacks_data);
}

auto solve_p2(std::string_view const data)
{
    auto data_view = data |
                     std::views::split(std::string_view{"\n\n"});

    auto data_iterator = data_view.begin();
    auto stacks_data = get_stacks_data(*(data_iterator++));
    auto const moves_data = get_moves_data(*data_iterator);

    move_crates_cratemover9001(stacks_data, moves_data);

    return get_result(stacks_data);
}

#include "../common/common.inl"
