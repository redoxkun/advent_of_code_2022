#include <algorithm>
#include <charconv>
#include <ranges>
#include <stack>
#include <string_view>
#include <vector>

auto get_ls_size(std::string_view const command) -> std::int64_t
{
    std::int64_t size = 0;

    for (auto const dir_entry : std::ranges::subrange(command.begin() + command.find('\n') + 1, command.end()) |
                                std::views::split('\n') |
                                std::views::transform([](auto const data) { return std::string_view {data.begin(), data.end()}; }))
    {
        if (std::int64_t file_size;
            std::from_chars(dir_entry.begin(), dir_entry.end() + dir_entry.find(' '), file_size).ec == std::errc{})
        {
            size += file_size;
        }
    }

    return size;
}

auto solve_p1(std::string_view const data)
{
    std::int64_t total_size = 0;
    std::stack<std::int64_t> sizes;
    sizes.push(0);
    for (auto const command : std::ranges::subrange(data.begin() + data.find('\n') + 2, data.end()) |
                              std::views::split('$') |
                              std::views::transform([](auto const data) { return std::string_view {data.begin(), data.end()}; }))
    {
        if (command.substr(1, 5) == "cd ..")
        {
            auto const dir_size = sizes.top();
            sizes.pop();
            if (dir_size < 100000)
                total_size += dir_size;
            sizes.top() += dir_size;
        }
        else if (command.substr(1, 2) == "cd")
            sizes.push(0);
        else if (command.substr(1, 2) == "ls")
            sizes.top() += get_ls_size(command);
    }

    while (not sizes.empty())
    {
        if (sizes.top() < 100000)
            total_size += sizes.top();
        sizes.pop();
    }

    return total_size;
}

void pop_and_update(std::vector<std::int64_t>& directories_sizes,
                    std::stack<std::int64_t>& sizes)
{
    auto const dir_size = sizes.top();
    directories_sizes.push_back(dir_size);
    sizes.pop();
    sizes.top() += dir_size;
}

auto get_directories_sizes(std::string_view const data) -> std::vector<std::int64_t>
{
    std::vector<std::int64_t> directories_sizes;

    std::stack<std::int64_t> sizes;
    sizes.push(0);
    for (auto const command : std::ranges::subrange(data.begin() + data.find('\n') + 2, data.end()) |
                              std::views::split('$') |
                              std::views::transform([](auto const data) { return std::string_view {data.begin(), data.end()}; }))
    {
        if (command.substr(1, 5) == "cd ..")
            pop_and_update(directories_sizes, sizes);
        else if (command.substr(1, 2) == "cd")
            sizes.push(0);
        else if (command.substr(1, 2) == "ls")
            sizes.top() += get_ls_size(command);
    }

    while (sizes.size() != 1)
        pop_and_update(directories_sizes, sizes);

    directories_sizes.push_back(sizes.top());

    return directories_sizes;
}

auto solve_p2(std::string_view const data)
{
    static auto constexpr disk_space   = 70000000;
    static auto constexpr needed_space = 30000000;

    auto directories_sizes = get_directories_sizes(data);

    auto const need_to_free_up_space = needed_space - (disk_space - directories_sizes.back());

    auto filter_view = directories_sizes |
                       std::views::filter([need_to_free_up_space](auto const size) { return size >= need_to_free_up_space; });

    std::vector<std::int64_t> filtered_sizes{filter_view.begin(), filter_view.end()};
    std::ranges::sort(filtered_sizes);

    return filtered_sizes.front();
}

#include "../common/common.inl"
