#include <fmt/core.h>

#include <array>
#include <charconv>
#include <ranges>
#include <set>
#include <string_view>

#include <cstdio>
#include <cstdlib>

auto get_motions_view(std::string_view const data)
{
    return data |
           std::views::split('\n') |
           std::views::transform([](auto const line_data)
                                 {
                                     std::string_view line{line_data.begin(), line_data.end()};
                                     if (int value; std::from_chars(line.begin() + 2, line.end(), value).ec == std::errc{})
                                         return std::pair{line[0], value};
                                     fmt::print(stderr, "Unknown value in a motion definition.");
                                     std::exit(1);
                                 });
}

auto operator+(std::pair<int, int> const& lhs, std::pair<int, int> const& rhs) -> std::pair<int, int>
{
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

auto operator-(std::pair<int, int> const& lhs, std::pair<int, int> const& rhs) -> std::pair<int, int>
{
    return {lhs.first - rhs.first, lhs.second - rhs.second};
}

template<int RopeSize>
class RopeSimulator
{
public:
    RopeSimulator()
    {
        for (auto i = 0; i < RopeSize; ++i)
            rope[i] = {0, 0};
    }

    void simulate_motion(char const direction, int const steps)
    {
        auto const offset = get_offset(direction);
        for (auto i = 0; i < steps; ++i)
        {
            rope[0] = rope[0] + offset;

            for (auto j = 1; j < RopeSize; ++j)
            {
                auto const movement_offset = rope[j - 1] - rope[j];

                if (std::abs(movement_offset.first) > 1 and std::abs(movement_offset.second) > 1)
                    rope[j] = {rope[j].first + movement_offset.first / 2, rope[j].second + movement_offset.second / 2};
                else if (std::abs(movement_offset.first) > 1)
                    rope[j] = {rope[j].first + movement_offset.first / 2, rope[j - 1].second };
                else if (std::abs(movement_offset.second) > 1)
                    rope[j] = {rope[j - 1].first, rope[j].second + movement_offset.second / 2};
            }

            tail_visited_positions.insert(rope[RopeSize - 1]);
        }
    }

    [[nodiscard]]
    auto get_number_of_tail_visited_positions() const -> std::size_t
    {
        return tail_visited_positions.size();
    }

private:
    std::array<std::pair<int, int>, RopeSize> rope;
    std::set<std::pair<int, int>> tail_visited_positions;

    static auto get_offset(char const direction) -> std::pair<int, int>
    {
        switch (direction)
        {
            case 'L': return {-1,  0};
            case 'R': return { 1,  0};
            case 'U': return { 0, -1};
            case 'D': return { 0,  1};
            default:
            {
                fmt::print(stderr, "Unknown motion command.");
                std::exit(1);
            }
        }
    }
};

auto solve_p1(std::string_view const data)
{
    RopeSimulator<2> simulator;

    for (auto const[direction, steps] : get_motions_view(data))
        simulator.simulate_motion(direction, steps);

    return simulator.get_number_of_tail_visited_positions();
}

auto solve_p2(std::string_view const data)
{
    RopeSimulator<10> simulator;

    for (auto const[direction, steps] : get_motions_view(data))
        simulator.simulate_motion(direction, steps);

    return simulator.get_number_of_tail_visited_positions();
}

#include "../common/common.inl"
