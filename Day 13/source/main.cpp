// Advent of code 2024 - Day 13
// https://adventofcode.com/2024/day/13
// By: Arthur Baurens

#include "AdventOfCode.hpp"

constexpr const char *const filepath = "assets/input.txt";

struct machine
{
  aoc::vec2l button_a;
  aoc::vec2l button_b;
  aoc::vec2l prize;

  friend std::istream &operator>>(std::istream &is, machine &m)
  {
    aoc::vec2 tmp;
    std::string line;
    
    while (std::getline(is, line) && line.empty());

    if (!std::sscanf(line.c_str(), "Button A: X+%d, Y+%d", &tmp.x, &tmp.y))
      throw std::string("Invalid input. Failed to read 'Button A' field.");
    m.button_a = tmp;

    std::getline(is, line);
    if (!std::sscanf(line.c_str(), "Button B: X+%d, Y+%d", &tmp.x, &tmp.y))
      throw std::string("Invalid input. Failed to read 'Button B' field.");
    m.button_b = tmp;

    std::getline(is, line);
    if (!std::sscanf(line.c_str(), "Prize: X=%d, Y=%d", &tmp.x, &tmp.y))
      throw std::string("Invalid input. Failed to read 'Prize' field.");
    m.prize = tmp;

    return is;
  }
};

static std::vector<machine> load_input(const char *const path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  std::vector<machine> machines;
  machine tmp;
  while (ifs >> tmp)
    machines.emplace_back(std::move(tmp));

  return machines;
}

// We have:
//   A = the A button's offset
//   B = the B button's offset
//   P = the prize's position
//
// We are searching for the smalest (i * 3 + j) such that:
//   i: a natural number
//   j: a natural number
//   iA + jB = P
//
// Decomposing the vectors, we end up with a system of 2 equations:
//  | iAx + jBx = Px |
//  | iAy + jBy = Py |
//
// This is solvable using Cramer's rule
static bool solve_machine(const machine &m, u64 &iv, u64 &jv)
{
  const aoc::vec2l &A = m.button_a;
  const aoc::vec2l &B = m.button_b;
  const aoc::vec2l &P = m.prize;

  const i64 det = (A.x * B.y - B.x * A.y);
  const i64 x = (P.x * B.y - B.x * P.y) / det;
  const i64 y = (A.x * P.y - P.x * A.y) / det;

  if (x < 0 || y < 0 || (A * x + B * y) != P)
    return false;

  iv = u64(x);
  jv = u64(y);

  return true;
}

void solve()
{
  std::vector<machine> machines = load_input(filepath);

  constexpr i64 part2_offset = 10'000'000'000'000;

  u64 tokens_part1 = 0;
  u64 tokens_part2 = 0;
  for (machine &m : machines)
  {
    u64 i = 0;
    u64 j = 0;
    if (solve_machine(m, i, j))
      tokens_part1 += (i * 3 + j);

    m.prize += part2_offset;
    if (solve_machine(m, i, j))
      tokens_part2 += (i * 3 + j);
  }

  aoc::cout << "Part 1 tokens: " << tokens_part1 << '\n';
  aoc::cout << "Part 2 tokens: " << tokens_part2 << '\n';
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
