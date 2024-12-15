// Advent of code 2024 - Day 12
// https://adventofcode.com/2024/day/12
// By: Arthur Baurens

#include "AdventOfCode.hpp"

constexpr const char *const filepath = "assets/input.txt";

enum direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

struct cell
{
  i8   type;
  bool fences[4] = { true, true, true, true };

  constexpr cell(i8 type) : type(type) {}

  constexpr u32 fence_count() const { return fences[0] + fences[1] + fences[2] + fences[3]; }

  constexpr bool operator==(const cell &other) const { return type == other.type; }
  constexpr bool operator!=(const cell &other) const { return type != other.type; }
};

using map = aoc::rect_map<cell>;
using region = std::unordered_set<aoc::vec2>;

static map load_map(const char *path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  map result;
  result.reserve(std::filesystem::file_size(path));
  ifs >> result;
  return result;
}

template<class T>
static void _floodfill(const aoc::rect_map<T> &map, std::unordered_set<aoc::vec2> &visited, const T &value, aoc::vec2 pos)
{
  if (!map.contains(pos) || visited.contains(pos))
    return;

  const T &cur = map[pos];
  if (cur != value)
    return;

  for (aoc::vec2 t = pos; t.x < map.width() && map[t] == value; ++t.x)
  {
    if (visited.contains(t))
      break;

    visited.insert(t);

    const aoc::vec2 up = { t.x, t.y - 1 };
    const aoc::vec2 down = { t.x, t.y + 1 };
    _floodfill(map, visited, value, up);
    _floodfill(map, visited, value, down);
  }

  for (aoc::vec2 t = pos; --t.x >= 0 && map[t] == value;)
  {
    if (visited.contains(t))
      break;

    visited.insert(t);

    const aoc::vec2 up = { t.x, t.y - 1 };
    const aoc::vec2 down = { t.x, t.y + 1 };
    _floodfill(map, visited, value, up);
    _floodfill(map, visited, value, down);
  }
}

template<class T>
static region floodfill(const aoc::rect_map<T> &map, aoc::vec2 pos)
{
  std::unordered_set<aoc::vec2> visited;

  const T &start = map[pos];
  _floodfill(map, visited, start, pos);
  return visited;
}

static std::vector<region> generate_regions(map &map)
{
  std::unordered_set<aoc::vec2> visited;
  std::vector<region> regions;

  for (int y = 0; y < map.height(); ++y)
  {
    for (int x = 0; x < map.width(); ++x)
    {
      const i8 type = map[aoc::vec2{x, y}].type;

      if (x > 0 && map[aoc::vec2{x - 1, y}].type == type)
      {
        map[aoc::vec2{ x, y}].fences[LEFT] = false;
        map[aoc::vec2{ x - 1, y}].fences[RIGHT] = false;
      }
      if (y > 0 && map[aoc::vec2{ x, y - 1}].type == type)
      {
        map[aoc::vec2{x, y}].fences[UP] = false;
        map[aoc::vec2{x, y - 1}].fences[DOWN] = false;
      }

      if (visited.contains({ x, y }))
        continue;

      region &&region = floodfill(map, { int(x), int(y) });
      visited.insert(region.begin(), region.end());
      regions.emplace_back(std::move(region));
    }
  }

  return regions;
}

static u32 region_price(const map &map, const region &region)
{
  u32 price = 0;
  for (const aoc::vec2 pos : region)
  {
    const cell &cell = map[pos];
    price += u32(region.size()) * cell.fence_count();
  }
  return price;
}

static u32 region_discounted_price(const map &map, const region &region)
{
  // the right vector for a direction
  constexpr aoc::vec2 right_of[] = {
    {  1,  0 }, // UP -> RIGHT
    {  0,  1 }, // RIGHT -> DOWN
    { -1,  0 }, // DOWN -> LEFT
    {  0, -1 }  // LEFT -> UP
  };

  u32 sides = 0;

  for (direction dir = UP; dir <= LEFT; dir = direction(dir + 1))
  {
    std::unordered_set<aoc::vec2> processed;

    for (const aoc::vec2 pos : region)
    {
      if (processed.contains(pos))
        continue;

      const cell &cell = map[pos];
      if (!cell.fences[dir])
        continue;

      // we found a fence in our current direction. find all connected fences
      for (aoc::vec2 n = pos; region.contains(n) && map[n].fences[dir]; n += right_of[dir])
      {
        if (processed.contains(n))
          break;
        processed.insert(n);
      }

      for (aoc::vec2 n = pos - right_of[dir]; region.contains(n) && map[n].fences[dir]; n -= right_of[dir])
      {
        if (processed.contains(n))
          break;
        processed.insert(n);
      }
      ++sides;
    }
  }
  return u32(region.size()) * sides;
}

void solve()
{
  map map = load_map(filepath);

  const std::vector<region> regions = generate_regions(map);

  u32 price = 0;
  for (const region &region : regions)
    price += region_price(map, region);
  aoc::cout << "Fencing price: " << price << '\n';

  u32 discounted_price = 0;
  for (const region &region : regions)
    discounted_price += region_discounted_price(map, region);
  aoc::cout << "Fencing discounted price: " << discounted_price << '\n';

}

void init()
{
  aoc::register_problem(DAY_NAME);
}
