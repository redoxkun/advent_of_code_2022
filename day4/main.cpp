#include "../common/common.inl"

#include <charconv>
#include <numeric>
#include <ranges>
#include <string_view>

#include <cassert>

struct SectionRange
{
    int first;
    int last;

    template<std::ranges::view ViewType>
    explicit SectionRange(ViewType view)
    {
        auto iterator = view.begin();
        first = *(iterator++);
        last  = *iterator;
    }

    [[nodiscard]]
    auto contains(SectionRange const& other) const -> bool
    {
        return other.first >= first and other.last <= last;
    }

    [[nodiscard]]
    auto overlaps(SectionRange const& other) const -> bool
    {
        return (other.first >= first and other.first <= last) or
               (other.last >= first  and other.last <= last) or
               (first >= other.first and first <= other.last) or
               (last >= other.first  and last <= other.last);
    }
};

auto get_sections_view(auto const line)
{
    return line |
           std::views::split(',') |
           std::views::transform([](auto const section)
           {
               auto section_limits_view = section |
                       std::views::split('-') |
                       std::views::transform([](auto const limit)
                       {
                           if (int value; std::from_chars(limit.begin(), limit.end(), value).ec == std::errc{})
                               return value;
                           assert(false);
                           return 0;
                       });
               return SectionRange{section_limits_view};
           });
}

auto solve_p1(std::string_view const data) -> int
{
    auto result_view = data |
            std::views::split('\n') |
            std::views::transform([](auto const line)
            {
                auto sections_view = get_sections_view(line);

                auto iterator = sections_view.begin();
                auto first_section = *(iterator++);
                auto second_section = *iterator;

                return (first_section.contains(second_section) or second_section.contains(first_section)) ? 1 : 0;
            });

    return std::reduce(result_view.begin(), result_view.end());
}

auto solve_p2(std::string_view const data) -> int
{
    auto result_view = data |
            std::views::split('\n') |
            std::views::transform([](auto const line)
            {
                auto sections_view = get_sections_view(line);

                auto iterator = sections_view.begin();
                auto first_section = *(iterator++);
                auto second_section = *iterator;

                return first_section.overlaps(second_section) ? 1 : 0;
            });

    return std::reduce(result_view.begin(), result_view.end());
}
