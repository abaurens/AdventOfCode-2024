// Advent of code 2024 - Day 08
// https://adventofcode.com/2024/day/8
// By: Arthur Baurens

#include "AdventOfCode.hpp"

constexpr const char *const filepath = "assets/input.txt";

using frequency = char;

using antenaes = std::map<frequency, std::vector<aoc::vec2>>;

struct map
{
  int width;
  int height;
  antenaes antenas;

  constexpr bool contains(const aoc::vec2 &pos) const
  {
    return pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < height;
  }
};

static map load_file(const char *path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  map result;

  int x;
  result.width = 0;
  result.height = 0;
  std::string line;
  while (std::getline(ifs, line))
  {
    if (result.width < line.size())
      result.width = int(line.size());
    else
      assert(line.size() == result.width);

    while ((x = int(line.find_first_not_of("."))) < line.size())
    {
      result.antenas[line[x]].emplace_back(x, result.height);
      line[x] = '.';
    }

    ++result.height;
  }

  return result;
}

void solve()
{
  const map map = load_file(filepath);

  aoc::vec2 point;
  std::set<aoc::vec2> antinodes;
  std::set<aoc::vec2> antinodes_ex;
  for (const antenaes::value_type &pair : map.antenas)
  {
    for (int i = 0; i < pair.second.size(); ++i)
    {
      for (int j = i + 1; j < pair.second.size(); ++j)
      {
        const aoc::vec2 diff = pair.second[j] - pair.second[i];

        point = pair.second[j];
        for (int l = 0; map.contains(point); ++l)
        {
          if (l == 1)
            antinodes.insert(point);
          antinodes_ex.insert(point);
          point = pair.second[j] + diff * l;
        }

        point = pair.second[j];
        for (int l = 0; map.contains(point); ++l)
        {
          if (l == 1)
            antinodes.insert(point);
          antinodes_ex.insert(point);
          point = pair.second[i] - diff * l;
        }
      }
    }
  }

  aoc::cout << "Antinodes: " << antinodes.size() << '\n';
  aoc::cout << "Harmonic antinodes: " << antinodes_ex.size() << '\n';
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
