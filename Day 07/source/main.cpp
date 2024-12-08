// Advent of code 2024 - Day 07
// https://adventofcode.com/2024/day/7
// By: Arthur Baurens

#include "AdventOfCode.hpp"

#include <regex>
#include <cerrno>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <execution>
#include <filesystem>

constexpr const char *const filepath = "assets/input.txt";

constexpr const char *const rxs = R"(([0-9]+):[ \t]+((?: ?[0-9]+)+))";

using num = uint64_t;
using num_list = std::vector<num>;

struct equation
{
  num       target = 0;
  num_list  operands;

  equation() = default;
  equation(const equation &) = default;
  equation(equation &&other) = default;
};
using equation_list = std::vector<equation>;

#if 0
#include <functional>

using op = std::function<num(num, num)>;

template<int N>
static num evaluate_equation(const equation &eq, uint32_t operators)
{
  const op op_funcs[] = {
    /* add */ [](num a, num b) { return a + b; },
    /* sub */ [](num a, num b) { return a * b; },
    /* cat */ [](num a, num b) { for (num n = b; n /= 10;) a *= 10; return (a * 10) + b; }
  };

  num result = eq.operands.front();
  for (size_t i = 1; i < eq.operands.size(); ++i)
  {
    if (result > eq.target)
      return 0;

    result = op_funcs[operators % N](result, eq.operands[i]);
    operators /= N;
  }

  return result;
}

template<int N>
static bool check_equation(const equation &eq)
{
  uint32_t max;
  if constexpr (N == 2)
    max = 1 << (eq.operands.size() - 1);
  else
    max = static_cast<uint32_t>(std::pow(N, eq.operands.size() - 1));

  for (uint32_t operators = 0; operators < max; ++operators)
  {
    const num res = evaluate_equation<N>(eq, operators);
    if (res == eq.target)
      return true;
  }
  return false;
}
#endif

// recursive variants here are both faster than iterative ones (mainly because they can early out faster).
static bool binary_search(const equation &eq, num result, int idx)
{
  if (result > eq.target)
    return false;

  if (idx == eq.operands.size())
    return result == eq.target;

  return binary_search(eq, result + eq.operands[idx], idx + 1) || binary_search(eq, result * eq.operands[idx], idx + 1);
}

static bool ternary_search(const equation &eq, num result, int idx)
{
  auto concat_prep = [](num a, num b) { for (num n = b; n /= 10;) a *= 10; return (a * 10) + b; };

  if (result > eq.target)
    return false;

  if (idx == eq.operands.size())
    return result == eq.target;

  return ternary_search(eq, result + eq.operands[idx], idx + 1) ||
         ternary_search(eq, result * eq.operands[idx], idx + 1) ||
         ternary_search(eq, concat_prep(result, eq.operands[idx]), idx + 1);
}

static equation_list parse_input(const char *path)
{
  equation_list eqs;
  equation      eq;

  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  const std::regex rx(rxs);

  std::string line;
  std::smatch match;
  while (std::getline(ifs, line))
  {
    if (!std::regex_match(line, match, rx))
      throw "Invalid input line: " + line;

    eq.operands.clear();

    eq.target = std::stoull(match[1]);
    std::stringstream ss(match[2]);

    for (num operand; ss >> operand; )
      eq.operands.emplace_back(std::move(operand));

    eqs.emplace_back(std::move(eq));
  }

  return eqs;
}

void solve()
{
  const equation_list eqs = parse_input(filepath);

  std::vector<num> results(eqs.size(), 0);

  std::transform(
    std::execution::par,
    eqs.cbegin(),
    eqs.cend(),
    results.begin(),
    [](const equation &eq) { return eq.target * binary_search(eq, eq.operands[0], 1); }
  );
  const num binary = std::reduce(std::execution::par, results.begin(), results.end());

  std::transform(
    std::execution::par,
    eqs.cbegin(),
    eqs.cend(),
    results.begin(),
    [](const equation &eq) { return eq.target * ternary_search(eq, eq.operands[0], 1); }
  );
  const num ternary = std::reduce(std::execution::par, results.begin(), results.end());

  aoc::cout << "Sum of solvable (A): " << binary << '\n';
  aoc::cout << "Sum of solvable (B): " << ternary << '\n';
}

void init()
{
  aoc::register_problem(DAY_NAME);
}