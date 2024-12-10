// Advent of code 2024 - Day 09
// https://adventofcode.com/2024/day/9
// By: Arthur Baurens

#include "AdventOfCode.hpp"

#include <deque>
#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string_view>

constexpr const char *const filepath = "assets/example.txt";

constexpr size_t read_size = 4096;

using slot = int16_t;
struct region
{
  slot   id    = -1;
  size_t count = 0;

  explicit constexpr operator bool() const { return id >= 0; }
};

using memory = std::deque<region>;

static size_t read(std::istream &is, char *buffer, size_t size)
{
  is.read(buffer, size);
  return is.gcount();
}

memory load_input(const char *const path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  memory result;
  //result.reserve(std::filesystem::file_size(path));

  slot id = 0;
  size_t read = 0;
  bool isFileBlock = true;
  char buffer[read_size + 1];
  while ((read = ::read(ifs, buffer, read_size)) > 0)
  {
    buffer[read] = '\0';

    for (char c : std::string_view(buffer))
    {
      if (c < '0' || c > '9')
        continue;

      if (isFileBlock)
        result.emplace_back(id++, c - '0');
      else //if ((c - '0') > 0)
        result.emplace_back(-1, c - '0');

      isFileBlock = !isFileBlock;
    }
  }

  if (result.back().id != -1)
    result.emplace_back(-1, 0);

  return result;
}


bool move_to_empty(memory &mem, int &src_pos, int dest_pos, bool allow_fragmentation = true)
{
  region &src = mem[src_pos];
  region &dest = mem[dest_pos];

  assert(src && !dest);

  if (dest.count < src.count && !allow_fragmentation)
    return false;

  if (src_pos == (dest_pos + 1))
  {
    std::swap(mem[src_pos++], mem[dest_pos]);
    return true;
  }

  const size_t moved = std::min(mem[dest_pos].count, src.count);

  // set [dest] to it's new value
  dest.id = src.id;
  if (dest.count > src.count)
  {
    mem.insert(mem.begin() + dest_pos + 1, { -1, dest.count - src.count });
    mem[dest_pos].count = moved;
    ++src_pos; // an insert happened. So the index increased
  }

  // move empty space from [src] into the end of the memory
  if (mem[src_pos].count > moved)
  {
    mem[src_pos].count -= moved;
    mem.insert(mem.begin() + src_pos + 1, { -1, moved });
  }
  else
    mem[src_pos].id = -1;
  return true;
}

//constexpr size_t positive_sum(size_t n)
//{
//  return n * (n + 1) / 2;
//}

size_t hash_memory(const memory &mem)
{
  size_t hash = 0;

  size_t i = 0;
  for (const region &reg : mem)
  {
    if (!reg)
    {
      i += reg.count;
      continue;
    }

    //hash += reg.id * (positive_sum(i + reg.count) - positive_sum(i));
    //i += reg.count;
    for (size_t j = 0; j < reg.count; ++j)
      hash += (i++ * reg.id);
  }

  return hash;
}

void merge_memory(memory &mem)
{
  for (size_t i = mem.size(); i-- > 0;)
  {
    while (i > 0 && mem[i].id == mem[i - 1].id)
    {
      mem[i - 1].count += mem[i].count;
      mem.erase(mem.begin() + i--);
    }
  }
}

static size_t solve_fragmented(memory mem)
{
  int free = 0;
  int last = int(mem.size() - 1);

  while (free < last)
  {
    while (mem[free]) // while free points to a file block (non empty)
      ++free;
    while (!mem[last]) // while last points to empty block
      --last;

    move_to_empty(mem, last, free);
    //merge_memory(mem);
    last = std::min(last, int(mem.size() - 1));
  }
  return hash_memory(mem);
}

static size_t solve_unfragmented(memory &mem)
{
  memory::iterator src = mem.end() - 1;

  while (src != mem.begin())
  {
    while (!*src) --src; // skip any empty space

    memory::iterator dest = mem.begin();
    for (; dest != src; ++dest)
    {
      if (*dest) // skip any non-empty region
        continue;
      if (dest->count < src->count) // skip any region that is too small
        continue;

      int src_pos = int(std::distance(mem.begin(), src));
      if (move_to_empty(mem, src_pos, int(std::distance(mem.begin(), dest)), false))
      {
        //merge_memory(mem);
        src = mem.begin() + src_pos;
        break;
      }
    }
    --src;
  }

  return hash_memory(mem);
}

void solve()
{
  memory mem = load_input(filepath);

  aoc::cout << "Fragmented checksum: " << solve_fragmented(mem) << '\n';
  aoc::cout << "Unfragmented checksum: " << solve_unfragmented(mem) << '\n';
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
