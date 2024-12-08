// Advent of code 2024 - Day 03
// https://adventofcode.com/2024/day/3
// By: Arthur Baurens

#include "AdventOfCode.hpp"

#include <cerrno>

#include <tuple>
#include <regex>
#include <vector>
#include <fstream>
#include <numeric>
#include <filesystem>

constexpr const char *const filepath = "assets/input.txt";

#define do_regex   R"(do\(\))"
#define dont_regex R"(don't\(\))"
#define mul_regex  R"(mul\(([0-9]{1,3}),([0-9]{1,3})\))"

constexpr const char *const complete_regex = "(?:" do_regex "|" dont_regex "|" mul_regex ")";

constexpr const char *const valid_chars = "mul(01234,56789)don't";

constexpr size_t buffer_size = 4096;

using Pair = std::tuple<bool, int, int>;
using PairList = std::vector<Pair>;

static void load_input(const char *path, PairList &pairs)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  pairs.clear();

  const std::regex re(complete_regex);

  size_t pos = 0;
  char   buffer[buffer_size + 1];

  bool enabled = true;
  do
  {
    ifs.read(buffer + pos, buffer_size - pos);
    char *end = buffer + pos + ifs.gcount();
    *end = 0;

    auto words_begin = std::cregex_iterator(buffer, end, re);
    auto words_end = std::cregex_iterator();

    pos = 0;
    for (std::cregex_iterator i = words_begin; i != words_end; ++i)
    {
      const std::cmatch match = *i;

      pos = match.position() + match.length();

      if (match.str() == "do()")
        enabled = true;
      else if (match.str() == "don't()")
        enabled = false;
      else
      {
        const int a = std::stoi(match[1]);
        const int b = std::stoi(match[2]);

        pairs.emplace_back(enabled, a, b);
      }
    }

    const std::string_view remaining(buffer + pos, buffer_size - pos);
    const size_t i = remaining.find_last_not_of(valid_chars);
    if (i < buffer_size)
      pos += (i + 1);

    memmove(buffer, buffer + pos, buffer_size - pos + 1);
    pos = buffer_size - pos;
  } while (!ifs.eof());
}

void solve()
{
  PairList pairs;

  load_input(filepath, pairs);

  const int mult_sum = std::transform_reduce(
    pairs.begin(), pairs.end(),
    0,
    std::plus{},
    [](const Pair &p) { return std::get<1>(p) * std::get<2>(p); }
  );

  const int filtered_sum = std::transform_reduce(
    pairs.begin(), pairs.end(),
    0,
    std::plus{},
    [](const Pair &p) { return std::get<0>(p) ? std::get<1>(p) * std::get<2>(p) : 0; }
  );

  aoc::cout << "Complete sum : " << mult_sum << '\n';
  aoc::cout << "Filtered sum : " << filtered_sum << '\n';
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
