#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

auto get_trees_matrix(std::string_view const data) -> std::vector<std::string_view>
{
    std::vector<std::string_view> trees;
    std::ranges::transform(data | std::views::split('\n'), std::back_inserter(trees),
                           [](auto const line) { return std::string_view{line.begin(), line.end()}; });
    return trees;
}

template<std::ranges::view TypeView>
auto visibility_check(TypeView const view, std::size_t x, std::size_t y, std::vector<std::string_view> const& trees) -> bool
{
    if (view.empty())
        return true;
    return std::ranges::max(view) < trees[y][x];
}

auto is_visible(std::size_t x, std::size_t y, std::vector<std::string_view> const& trees) -> bool
{
    return visibility_check(std::ranges::subrange(trees[y].begin(), trees[y].begin() + x), x, y, trees) or
           visibility_check(std::ranges::subrange(trees[y].begin() + x + 1, trees[y].end()), x, y, trees) or
           visibility_check(std::ranges::subrange(trees.begin(), trees.begin() + y) |
                            std::views::transform([x](auto const line) { return line[x]; }), x, y, trees) or
           visibility_check(std::ranges::subrange(trees.begin() + y + 1, trees.end()) |
                            std::views::transform([x](auto const line) { return line[x]; }), x, y, trees);
}

auto solve_p1(std::string_view const data)
{
    auto const trees = get_trees_matrix(data);
    auto visible_trees = 0;

    for (std::size_t y = 0; y < trees.size(); ++y)
    {
        for (std::size_t x = 0; x < trees[y].size(); ++x)
            visible_trees += is_visible(x, y, trees) ? 1 : 0;
    }

    return visible_trees;
}

template<std::ranges::view TypeView>
auto get_same_height_or_taller_distance(TypeView const view, std::size_t x, std::size_t y,
                                        std::vector<std::string_view> const& trees) -> int
{
    auto const tree_position = std::ranges::find_if(view, [height = trees[y][x]](auto const tree_height) {
                                                                                 return tree_height >= height; });
    return std::distance(view.begin(), tree_position) + (tree_position != view.end() ? 1 : 0);
}

auto get_scenic_score(std::size_t x, std::size_t y, std::vector<std::string_view> const& trees) -> int
{
    return get_same_height_or_taller_distance(std::ranges::subrange(trees[y].begin(), trees[y].begin() + x) |
                                              std::views::reverse, x, y, trees) *
           get_same_height_or_taller_distance(std::ranges::subrange(trees[y].begin() + x + 1, trees[y].end()), x, y, trees) *
           get_same_height_or_taller_distance(std::ranges::subrange(trees.begin(), trees.begin() + y) |
                                              std::views::reverse |
                                              std::views::transform([x](auto const line) {return line[x];}), x, y, trees) *
           get_same_height_or_taller_distance(std::ranges::subrange(trees.begin() + y + 1, trees.end()) |
                                              std::views::transform([x](auto const line) {return line[x];}), x, y, trees);
}

auto solve_p2(std::string_view const data)
{
    auto const trees = get_trees_matrix(data);
    auto max_scenic_score = 0;

    for (std::size_t y = 1; y < trees.size() - 1; ++y)
    {
        for (std::size_t x = 1; x < trees[y].size() - 1; ++x)
            max_scenic_score = std::max(max_scenic_score, get_scenic_score(x, y, trees));
    }

    return max_scenic_score;
}

#include "../common/common.inl"
