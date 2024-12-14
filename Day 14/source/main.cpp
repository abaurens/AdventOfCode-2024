// Advent of code 2024 - Day 14
// https://adventofcode.com/2024/day/14
// By: Arthur Baurens

#include "AdventOfCode.hpp"

constexpr const char *const filepath = "assets/input.txt";

constexpr aoc::vec2 map_size = { 101, 103 };

struct drone
{
  aoc::vec2 pos;
  aoc::vec2 vel;

  friend std::istream &operator>>(std::istream &is, drone &drone)
  {
    std::string line;

    while (std::getline(is, line) && line.empty());

    if (!std::sscanf(line.c_str(), "p=%d,%d v=%d,%d", &drone.pos.x, &drone.pos.y, &drone.vel.x, &drone.vel.y))
      throw std::string("Invalid input.");

    return is;
  }
};

static std::vector<drone> load_input(const char *const path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  std::vector<drone> drones;

  drone drone;
  while (ifs >> drone)
    drones.emplace_back(std::move(drone));

  return drones;
}

#if _DEBUG
aoc::rect_map map(map_size.w, map_size.h, ' ');

static void debug_map()
{
  aoc::vec2 pos;
  for (pos.y = 0; pos.y < map.height(); ++pos.y)
  {
    write(1, &map[pos], u32(map.width()));
    write(1, "\n", 1u);
  }
}
#endif

static u32 get_safety_factor(const std::vector<drone> &drones, i32 time = 0)
{
  // number of drones per quadrants
  std::atomic<u32> counts[4] = { 0, 0, 0, 0 };

  std::for_each(
    std::execution::par,
    drones.cbegin(), drones.cend(),
    [&counts, time](const drone &drone)
    {
      aoc::vec2 pos = (drone.pos + (drone.vel * time)) % map_size;
      const aoc::vec2 cong = (pos < 0) * map_size; // velocity can be negarive. So we need to correct the congruence
      pos += cong;

      // ignore robots that are in the center lines
      if (pos.x == map_size.x / 2 || pos.y == map_size.y / 2)
        return;

      const aoc::vec2b &&tmp = pos > (map_size / 2);
      const u8 quad = tmp.x + 2 * tmp.y;
      counts[quad]++;
    }
  );

  return counts[0] + counts[1] + counts[2] + counts[3];
}

static i32 find_easter_egg(const std::vector<drone> &drones)
{
  for (i32 time = 100; time < 10'000; ++time)
  {
    std::vector<aoc::vec2l> positions(drones.size(), { 0, 0 });

    aoc::vec2l center;

    // compute the drones positions
    std::transform(
      std::execution::par,
      drones.cbegin(), drones.cend(),
      positions.begin(),
      [&center, time](const drone &drone)
      {
        aoc::vec2l pos = (drone.pos + drone.vel * time) % map_size;
        const aoc::vec2 cong = (pos < 0) * map_size;
        pos += cong;
        
        center += pos; // pre-compute the average position opf the drones

        return pos;
      }
    );

    center /= drones.size();

    u64 avg_dist = 0;

    std::for_each(
      std::execution::par,
      positions.cbegin(), positions.cend(),
      [&center, &avg_dist](const aoc::vec2l &pos)
      {
        const aoc::vec2l &&diff = pos - center;
        avg_dist += diff.dot(diff);
      }
    );

    avg_dist /= drones.size();

    if (avg_dist < 1'000)
      return time;
  }

  return 0;
}

void solve()
{
  const std::vector<drone> drones = load_input(filepath);

  // number of drones per quadrants
  const u32 safety_factor = get_safety_factor(drones, 100);
  aoc::cout << "Safety factor: " << safety_factor << '\n';

  const i32 guess_time = find_easter_egg(drones);
  aoc::cout << "Next easter egg: " << guess_time << "\n";

 #if _DEBUG
  std::memset(map.data(), '.', map.width() * map.height());
  std::for_each(
    std::execution::par,
    drones.begin(), drones.end(),
    [guess_time](const drone &drone)
    {
      aoc::vec2l pos = (drone.pos + drone.vel * guess_time) % map_size;
      const aoc::vec2 cong = (pos < 0) * map_size;
      pos += cong;

      map[pos] = '#';
    }
  );
  debug_map();
 #endif
}

void init()
{
  aoc::register_problem(DAY_NAME);
}
