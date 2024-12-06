// Advent of code 2024 - Day 06
// https://adventofcode.com/2024/day/6
// By: Arthur Baurens

#include <cerrno>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <iostream>
#include <filesystem>
#include <string_view>

#if _DEBUG
# define DBG(...) __VA_ARGS__
#else
# define DBG(...)
#endif

constexpr const char *const filepath = "assets/input.txt";

constexpr const char guard_dirs[] = "^>v<";

constexpr const char empty_space = '.';
constexpr const char obscacle = '#';

#define BIT(x) (1 << x)

struct vec2
{
  int x = 0;
  int y = 0;

  bool operator==(const vec2 &other) const { return (x == other.x && y == other.y); }
  bool operator!=(const vec2 &other) const { return (x != other.x || y != other.y); }

  vec2 &operator+=(int scalar) { x += scalar;  y += scalar;  return *this; }
  vec2 &operator+=(vec2 other) { x += other.x; y += other.y; return *this; }

  vec2 &operator-=(int scalar) { x -= scalar;  y -= scalar;  return *this; }
  vec2 &operator-=(vec2 other) { x -= other.x; y -= other.y; return *this; }

  vec2 operator+(int scalar) const { return { x + scalar,  y + scalar  }; }
  vec2 operator+(vec2 other) const { return { x + other.x, y + other.y }; }

  vec2 operator-(int scalar) const { return { x - scalar,  y - scalar  }; }
  vec2 operator-(vec2 other) const { return { x - other.x, y - other.y }; }
};

class Direction
{
private:
  enum class Value : uint8_t
  {
    UP    = BIT(0),
    RIGHT = BIT(1),
    DOWN  = BIT(2),
    LEFT  = BIT(3)
  };

public:
  using enum Value;

  constexpr Direction(Value direction) : m_val(direction) {}
  constexpr Direction(char c) : m_val(UP)
  {
    static_assert(sizeof(guard_dirs) == 5, "CHARS must be a string of 4 characters");
    for (int i = 0; i < 4; ++i)
    {
      if (c == guard_dirs[i])
      {
        m_val = static_cast<Value>(BIT(i));
        break;
      }
    }
  }

  constexpr bool operator==(Direction other) const { return (m_val == other.m_val) != 0; }
  constexpr bool operator!=(Direction other) const { return (m_val != other.m_val) != 0; }

  constexpr operator char(void) const
  {
    switch (m_val)
    {
      case UP:    return guard_dirs[0];
      case RIGHT: return guard_dirs[1];
      case DOWN:  return guard_dirs[2];
      case LEFT:  return guard_dirs[3];
    }
    return '?';
  }

  constexpr operator int(void) const { return static_cast<uint8_t>(m_val); }

  constexpr operator vec2(void) const {
    switch (m_val)
    {
      case UP:    return {  0, -1 };
      case RIGHT: return {  1,  0 };
      case DOWN:  return {  0,  1 };
      case LEFT:  return { -1,  0 };
    }
    return { 0, 0 };
  }

  constexpr Direction operator+(int i)
  {
    if (i < 0)
      return operator-(-i);

    if (i > 4) i %= 4;

    const uint8_t val = static_cast<uint8_t>(m_val);
    return static_cast<Value>(((val << i) | (val >> (4 - i))) & 0x0f);
  }

  constexpr Direction operator-(int i)
  {
    if (i < 0)
      return operator+(-i);

    if (i > 4) i %= 4;

    const uint8_t val = static_cast<uint8_t>(m_val);
    return static_cast<Value>(((val >> i) | (val << (4 - i))) & 0x0f);
  }

  constexpr Direction &operator++(void)
  {
    const uint8_t val = static_cast<uint8_t>(m_val);
    m_val = static_cast<Value>(((val << 1) | (val >> 3)) & 0x0f);
    return *this;
  }

  constexpr Direction &operator--(void)
  {
    const uint8_t val = static_cast<uint8_t>(m_val);
    m_val = static_cast<Value>(((val >> 1) | (val << 3)) & 0x0f);
    return *this;
  }

private:
  Value m_val;
};

struct Guard
{
  vec2 pos;
  Direction dir = Direction::UP;
};

struct Map
{
private:
  struct MapPrinter
  {
    const Map &map;
    const Guard &guard;

    friend std::ostream &operator<<(std::ostream &os, const MapPrinter &printer)
    {
      const Map &map = printer.map;
      const Guard &guard = printer.guard;

      std::string data = map.data;
      for (size_t i = 0; i < data.size(); ++i)
      {
        if (data[i] <= 0x0f)
          data[i] = 'x';
      }

      std::string_view line;
      for (size_t y = 0; y < map.height; ++y)
      {
        line = std::string_view(&data[y * map.width], map.width);
        if (y != guard.pos.y)
          os << line;
        else
          os << line.substr(0, guard.pos.x) << char(guard.dir) << line.substr(guard.pos.x + 1);
        os << '\n';
      }

      return os;
    }
  };

public:
  size_t width = 0;
  size_t height = 0;
  std::string data;

  MapPrinter operator()(const Guard &guard) const { return MapPrinter(*this, guard); }

  char &operator[](const vec2 &pos) { return data[pos.x + pos.y * width]; }
  const char &operator[](const vec2 &pos) const { return data[pos.x + pos.y * width]; }

  bool contains_pos(const vec2 &pos) const { return (pos.x >= 0 && pos.y >= 0 && pos.x < width && pos.y < height); }

  // returns wether or not the guard is within the map's bounds
  bool operator[](const Guard &guard) const { return contains_pos(guard.pos); }
};


static size_t read(std::istream &is, char *buffer, size_t size)
{
  is.read(buffer, size);
  return is.gcount();
}

static char *strchrnul(const char *p, int c)
{
  while (*p && *p != c)
    ++p;
  return const_cast<char *>(p);
}

static Map load_file(const char *path, Guard &guard)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  bool found_guard = false;
  Map result;

  size_t      x;
  std::string line;
  while (std::getline(ifs, line))
  {
    if (!found_guard && (x = line.find_first_of(guard_dirs)) < line.size())
    {
      guard.pos.x = int(x);
      guard.pos.y = int(result.height);
      guard.dir = line[x];
      line[x] = empty_space;
      found_guard = true;
    }

    result.data.append(line);

    if (result.width == 0)
      result.width = line.size();
    else
      assert(line.size() == result.width);

    result.height++;
  }

  return result;
}

static bool play_turn(Map &map, Guard &guard)
{
  bool new_cell = false;
  char &cell = map[guard.pos];

  // Remember if the guard never visited this cell
  if (cell == empty_space)
  {
    new_cell = true;
    cell = 0;
  }
  // mark the cell as visited in that direction
  cell |= int(guard.dir);

  // get the guard's next position
  vec2 next = guard.pos + vec2(guard.dir);

  // either move forward or turn right
  if (map.contains_pos(next) && map[next] == obscacle)
    ++guard.dir;
  else 
    guard.pos = next;

  return new_cell;
}

static bool is_loop_oportunity(Map map, Guard guard, vec2 start = { -1, -1 })
{
  const vec2 front_pos = guard.pos + vec2(guard.dir);

  // we cannot place a box on the starting cell
  if (front_pos == start)
    return false;

  // if the guard is about to leave the map
  if (!map.contains_pos(front_pos))
    return false;

  // putting an obstacle here would change the already traced path
  if (map[front_pos] != empty_space)
    return false;

  // if the guard is already about to turn right
  //if (map[front_pos] == obscacle || front_pos == start)
  //  return false;

  //const Guard comp = guard;

  // place the obstacle and turn the guard right
  map[front_pos] = obscacle;
  map[guard.pos] = int(guard.dir);
  ++guard.dir;

  // simulate untill a loop is found or the guard leaves the map
  while (map[guard])
  {
    const char &cell = map[guard.pos];
    if (cell < 0x10 && cell & int(guard.dir))
    {
      DBG(std::cout << "\033[32m" << map(guard) << "\033[0m" << std::endl);
      return true;
    }

    play_turn(map, guard);
  }

  return false;
}

static void solve_input(const char *path)
{
  Guard guard;
  Map map = load_file(path, guard);

  const vec2 start = guard.pos;

  int visited_spaces = 0;
  int loop_oportunities = 0;

  // while the guard is within the map bounds
  while (map[guard])
  {
    loop_oportunities += is_loop_oportunity(map, guard);

    DBG(std::cout << map(guard) << std::endl);
    visited_spaces += play_turn(map, guard);
  }

  std::cout << "The guard visited " << visited_spaces << " cells." << std::endl;
  std::cout << "There is " << loop_oportunities << " loop oportunities." << std::endl;
}

int main()
{
  try
  {
    solve_input(filepath);
  }
  catch (std::string &err)
  {
    std::cerr << "Error: " << err << std::endl;
    return 1;
  }

  return 0;
}
