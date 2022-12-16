#include <fmt/core.h>

#include <charconv>
#include <ranges>
#include <string_view>
#include <utility>
#include <vector>

#include <cassert>
#include <cstdio>
#include <cstdlib>

class SimpleCpu
{
private:
    enum class InstructionOpCode
    {
        noop,
        addx,
    };

    struct Instruction
    {
        InstructionOpCode op_code;
        std::int32_t parameter1;
    };

public:
    explicit SimpleCpu(std::string_view const code)
    {
        load(code);
    }

    void run(std::int32_t const cycles)
    {
        for (auto i = 0; i < cycles; ++i)
        {
            assert(PC < instructions.size());
            switch (instructions[PC].op_code) {
                case InstructionOpCode::noop:
                    ++PC;
                    break;
                case InstructionOpCode::addx:
                    execute_addx(instructions[PC].parameter1);
                    break;
                default:
                    std::unreachable();
            }
        }
    }

    [[nodiscard]]
    auto getX() const -> std::int32_t
    {
        return X;
    }

private:
    std::int32_t X  = 1;

    std::int32_t PC = 0;
    std::int32_t IC = 0;

    std::vector<Instruction> instructions;

    void execute_addx(std::int32_t const value)
    {
        if (IC == 0)
            IC = 1;
        else
        {
            X += value;
            ++PC;
            IC = 0;
        }
    }

    void load(std::string_view const code)
    {
        auto instructions_view = code |
                                 std::views::split('\n') |
                                 std::views::transform([](auto const line_data)
                                 {
                                     std::string_view line{line_data.begin(), line_data.end()};
                                     if (line == "noop")
                                         return Instruction{InstructionOpCode::noop, 0};
                                     else if (int value; std::from_chars(line.begin() + 5, line.end(), value).ec == std::errc{})
                                         return Instruction{InstructionOpCode::addx, value};

                                     fmt::print(stderr, "Unknown instruction.");
                                     std::exit(1);
                                 });
        instructions = {instructions_view.begin(), instructions_view.end()};
    }
};

auto solve_p1(std::string_view const data)
{
    SimpleCpu cpu{data};

    cpu.run(19); // ???
    auto strengths = cpu.getX() * 20;
    cpu.run(40);
    strengths += cpu.getX() * 60;
    cpu.run(40);
    strengths += cpu.getX() * 100;
    cpu.run(40);
    strengths += cpu.getX() * 140;
    cpu.run(40);
    strengths += cpu.getX() * 180;
    cpu.run(40);
    strengths += cpu.getX() * 220;

    return strengths;
}

auto solve_p2(std::string_view const data)
{
    std::string crt_image;

    SimpleCpu cpu{data};

    for (auto i = 0; i < 6; ++i)
    {
        for (auto j = 0; j < 40; ++j)
        {
            auto const value = cpu.getX();
            cpu.run(1);
            crt_image.push_back(((j >= value - 1) and (j <= value + 1)) ? '#' : '.' );
        }
        crt_image.push_back('\n');
    }

    return crt_image;
}

#include "../common/common.inl"
