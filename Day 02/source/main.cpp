// Advent of code 2024 - Day 02
// https://adventofcode.com/2024/day/2
// By: Arthur Baurens

#include <cerrno>

#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>

using Level = uint8_t;

using Report = std::vector<Level>;

constexpr Level SAFETY_TRESHOLD = 3;

constexpr const char *const filepath = "assets/input.txt";

static bool is_safe_report(const Report &report, int ignored = -1)
{
  bool increasing = true;
  bool decreasing = true;

  if (report.size() < 2)
    return true;

  const bool skip_first = (ignored == 0);

  Level a = report[skip_first];
  for (int i = 1 + skip_first; i < report.size(); ++i)
  {
    if (i == ignored)
      continue;

    const Level b = report[i];

    if (a == b || abs(a - b) > SAFETY_TRESHOLD)
      return false;

    if (a > b)
      increasing = false;

    if (a < b)
      decreasing = false;

    a = b;
  }

  return (increasing || decreasing);
}

static bool is_dampened_safe_report(const Report &report)
{
  for (int ign = 0; ign < report.size(); ++ign)
    if (is_safe_report(report, ign))
      return true;
  return false;
}

int main()
{
  if (!std::filesystem::exists(filepath))
    throw "Input file " + std::string(filepath) + " does not exist";

  std::ifstream ifs(filepath);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  size_t safeCount = 0;
  size_t dampenedSafeCount = 0;

  Report report;

  int val;
  while (ifs >> val)
  {
    report.emplace_back(val);

    const char next = ifs.peek();
    if (next != '\n' && !ifs.eof())
      continue;

    if (is_safe_report(report))
      ++safeCount;
    else
      dampenedSafeCount += is_dampened_safe_report(report);

    report.clear();
  }

  dampenedSafeCount += safeCount;

  std::cout << safeCount << " safe reports." << std::endl;
  std::cout << dampenedSafeCount << " dampened safe reports." << std::endl;
  return 0;
}
