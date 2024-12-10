// Advent of code 2024 - Day 10
// https://adventofcode.com/2024/day/10
// By: Arthur Baurens

#include "AdventOfCode.hpp"

#include <set>
#include <cassert>
#include <fstream>
#include <numeric>
#include <concepts>
#include <iostream>
#include <filesystem>

constexpr const char *const filepath = "assets/input.txt";

enum dir
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

dir operator!(const dir &d)
{
  return dir((d + 2) % 4);
}

struct Node
{
  uint8_t height;
  Node *neighbors[4] = { nullptr };

  Node(int height) : height(height) {}
  Node(char height) : height(height >= '0' ? height - '0' : height) {}

  operator char() const { return height < 10 ? height + '0' : '.'; }
};

struct map
{
  size_t width = 0;
  size_t height = 0;
  std::vector<Node> nodes;
};

using Results = std::pair<size_t, size_t>;

template<class T>
constexpr bool is_result_pair = false;
template<>
constexpr bool is_result_pair<Results> = true;

void link_nodes(Node &a, Node &b, dir d)
{
  if (b.height == (a.height + 1))
    a.neighbors[d] = &b;
  else if (a.height == (b.height + 1))
    b.neighbors[!d] = &a;
}

map load_input(const char *path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  map result;
  result.nodes.reserve(std::filesystem::file_size(path));

  std::string line;
  while (std::getline(ifs, line))
  {
    if (result.width)
      assert(line.size() == result.width);
    else
      result.width = line.size();

    for (size_t x = 0; x < result.width; ++x)
    {
      Node &n = result.nodes.emplace_back(line[x]);
      if (x > 0)
        link_nodes(n, result.nodes[(x - 1) + result.height * result.width], LEFT);
      if (result.height > 0)
        link_nodes(n, result.nodes[x + (result.height - 1) * result.width], UP);
    }

    ++result.height;
  }

  return result;
}

size_t _get_score(const map &map, const Node *node, std::set<const Node *> &visited)
{
  static auto available = [&visited](const Node &n)
  {
    return (
        (!visited.contains(n.neighbors[0]) && n.neighbors[0])
      + (!visited.contains(n.neighbors[1]) && n.neighbors[1])
      + (!visited.contains(n.neighbors[2]) && n.neighbors[2])
      + (!visited.contains(n.neighbors[3]) && n.neighbors[3])
    );
  };

  size_t avail;
  while ((avail = available(*node)) == 1)
  {
    size_t i;
    for (i = 0; i < 4 && !node->neighbors[i] || visited.contains(node->neighbors[i]); ++i);
    visited.insert(node);
    node = node->neighbors[i];
  }
  visited.insert(node);

  if (node->height == 9)
    return 1;

  if (avail == 0)
    return 0;

  size_t result = 0;
  for (dir d = UP; d < 4; d = dir(d + 1))
  {
    const Node *neighbor = node->neighbors[d];
    if (neighbor && !visited.contains(neighbor))
      result += _get_score(map, neighbor, visited);
  }
  return result;
}

size_t get_score(const map &map, const Node *node)
{
  std::set<const Node *> visited;
  return _get_score(map, node, visited);
}

size_t get_rating(const map &map, const Node *node)
{
  static auto available = [](const Node &n)
  {
    return !!n.neighbors[0] + !!n.neighbors[1] + !!n.neighbors[2] + !!n.neighbors[3];
  };

  size_t avail;
  while ((avail = available(*node)) == 1)
  {
    size_t i;
    for (i = 0; i < 4 && !node->neighbors[i]; ++i);
    node = node->neighbors[i];
  }

  if (node->height == 9)
    return 1;

  if (avail == 0)
    return 0;

  size_t result = 0;
  for (dir d = UP; d < 4; d = dir(d + 1))
  {
    const Node *neighbor = node->neighbors[d];
    if (neighbor)
      result += get_rating(map, neighbor);
  }
  return result;
}

#if _DEBUG
void debug_map(const map &map)
{
  for (size_t y = 0; y < map.height; ++y)
  {
    if (y > 0)
    {
      const size_t yup = y - 1;
      for (size_t x = 0; x < map.width; ++x)
      {
        if (x > 0)
          std::cout << ' ';
        const Node &n = map.nodes[x + y * map.width];
        const Node &up = map.nodes[x + yup * map.width];
        if (n.neighbors[UP])
          std::cout << '^';
        else if (up.neighbors[DOWN])
          std::cout << 'v';
        else
          std::cout << ' ';
      }
      std::cout << '\n';
    }

    for (size_t x = 0; x < map.width; ++x)
    {
      const Node &n = map.nodes[x + y * map.width];

      if (x > 0)
      {
        const Node &left = map.nodes[(x - 1) + y * map.width];

        if (n.neighbors[LEFT])
          std::cout << '<';
        else if (left.neighbors[RIGHT])
          std::cout << '>';
        else
          std::cout << ' ';
      }
      std::cout << map.nodes[x + y * map.width];
    }
    std::cout << '\n';
  }
}
#endif

void solve()
{
  const map map = load_input(filepath);

 #if _DEBUG
  debug_map(map);
 #endif

  std::vector<const Node *> starts;
  for (size_t i = 0; i < map.nodes.size(); ++i)
    if (map.nodes[i].height == 0)
      starts.push_back(&map.nodes[i]);

  size_t trail_score = 0;
  size_t trail_rating = 0;
  
  for (const Node *start : starts)
  {
    trail_score += get_score(map, start);
    trail_rating += get_rating(map, start);
  }

  aoc::cout << "Trail Score  : " << trail_score << '\n';
  aoc::cout << "Trail rating : " << trail_rating << '\n';
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
