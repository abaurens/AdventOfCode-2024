// Advent of code library
// https://adventofcode.com
// By: Arthur Baurens

#include "AdventOfCode.hpp"

#include "Timer.hpp"
#include "Output.hpp"

#include <cerrno>
#include <chrono>
#include <cassert>
#include <iostream>

using ss = std::stringstream;

aoc::Output aoc::cout;

constexpr int l_margin = 1;
constexpr int r_margin = 3;

static void print_centered(const std::string &str, size_t width)
{
  width += l_margin + r_margin;

  const size_t l_padding = (width - str.size()) / 2;
  const size_t r_padding = width - (l_padding + str.size());

  std::cout
    << '|'
    << std::setw(l_padding) << std::left  << ""
    << str
    << std::setw(r_padding) << std::right << ""
    << "|\n";
}

static void print_left(const std::string &str, size_t width)
{
  width += l_margin + r_margin;

  const size_t l_padding = l_margin;
  const size_t r_padding = width - (l_padding + str.size());

  std::cout
    << '|'
    << std::setw(l_padding) << std::left << ""
    << str
    << std::setw(r_padding) << std::right << ""
    << "|\n";
}

static void display_result(uint64_t micros)
{
  const char *unit = "seconds";
  double time = double(micros / 1'000'000.0);

  if (micros < 1000)
  {
    unit = "milliseconds";
    time = double(micros / 1'000.0);
  }

  const std::string HEADER  = (PROJECT_NAME " : " + aoc::ProblemName());
  const std::string SUMMARY = (ss() << "Problem solved in " << std::setprecision(3) << time << ' ' << unit).str();

  aoc::cout.flush();

  size_t width = aoc::cout.width();
  if (SUMMARY.size() > width)
    width = SUMMARY.size();

  const std::string border(width + l_margin + r_margin, '-');

  // header
  std::cout << '+' << border << "+\n";
  print_centered(HEADER, width);

  // problem solver output
  std::cout << '+' << border << "+\n";
  print_left("", width);
  for (const std::string &line : aoc::cout)
    print_left(line, width);
  if (aoc::cout)
    print_left("", width);

  // summary
  std::cout << '+' << border << "+\n";
  print_centered(SUMMARY, width);
  std::cout << '+' << border << "+\n";
}

int main()
{
  try
  {
    init();

    aoc::Timer timer;
    solve();
    display_result(timer.GetTime<std::chrono::microseconds>());
  }
  catch (std::string &err)
  {
    std::cerr << "Error: " << err << std::endl;
    return 1;
  }

  return 0;
}
