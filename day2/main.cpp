#include <numeric>
#include <ranges>
#include <string_view>

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

auto solve_p1(std::string_view const data)
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

auto solve_p2(std::string_view data)
{
    auto results_view = data |
            std::views::split('\n') |
            std::views::transform([](auto const value)
            {
                return get_points_p2({value.begin(), value.end()});
            });

    return std::reduce(results_view.begin(), results_view.end());
}

#include "../common/common.inl"
