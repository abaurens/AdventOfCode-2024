// Advent of code 2024 - Day 11
// https://adventofcode.com/2024/day/11
// By: Arthur Baurens

#include "AdventOfCode.hpp"

using Stone = uint64_t;
using List = std::vector<Stone>;

constexpr const char *const filepath = "assets/input.txt";

constexpr Stone odd_factor = 2024;

static int num_len(size_t num)
{
  int len = 1;

  while (num / 10)
  {
    num /= 10;
    ++len;
  }
  return len;
};

static List load_input(const char *path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  size_t n;
  List result;
  while (ifs >> n)
    result.emplace_back(n);

  return result;
}

static aoc::cvector<Stone, 2> blink_at(const Stone &stone)
{
  if (stone == 0)
    return { 1 };

  int len = num_len(stone);
  if (len % 2)
    return { stone * odd_factor };

  len /= 2;

  Stone left = stone;
  size_t power = 1;
  for (int i = 0; i < len; ++i)
    power *= 10;
  left /= power;

  return { left, stone - left * power };
}

static size_t blink(const List &stones, size_t turn)
{
  std::unordered_map<Stone, std::size_t> counter;
  for (const Stone &stone : stones)
    counter[stone] = 1;

  std::unordered_map<Stone, std::size_t> counter_next;
  for (size_t i = 0; i < turn; ++i)
  {
    for (const auto &[stone, count] : counter)
    {
      for (const Stone &s : blink_at(stone))
        counter_next[s] += count;
    }
    counter = std::move(counter_next);
    counter_next.clear();
  }

  size_t result = 0;
  for (const auto &[_, count] : counter)
    result += count;
  return result;
}

void solve()
{
  const List stones = load_input(filepath);

  aoc::cout << blink(stones, 25) << " stones after 25 blinks.\n";
  aoc::cout << blink(stones, 75) << " stones after 75 blinks.\n";
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
