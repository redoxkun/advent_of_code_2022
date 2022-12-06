#include <fmt/core.h>

#include <string>
#include <string_view>

#include <cstdio>
#include <cstdlib>

auto get_processed_characters(std::string_view const data, std::size_t const distinct)
{
    std::string buffer;
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        if (auto const position = buffer.find(data[i]); position != std::string::npos)
        {
            buffer.erase(buffer.begin(), buffer.begin() + position + 1);
            buffer.push_back(data[i]);
            continue;
        }

        buffer.push_back(data[i]);

        if (buffer.size() == distinct)
            return i + 1;
    }

    fmt::print(stderr, "Marker not found.\n");
    std::exit(1);
}

auto solve_p1(std::string_view const data)
{
    return get_processed_characters(data, 4);
}

auto solve_p2(std::string_view const data)
{
    return get_processed_characters(data, 14);
}

#include "../common/common.inl"
