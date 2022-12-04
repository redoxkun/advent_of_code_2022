#include <algorithm>
#include <numeric>
#include <ranges>
#include <string_view>
#include <vector>

#include "../common/common.inl"

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
