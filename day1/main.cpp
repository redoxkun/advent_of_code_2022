#include <charconv>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

#include "../common/common.inl"

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
