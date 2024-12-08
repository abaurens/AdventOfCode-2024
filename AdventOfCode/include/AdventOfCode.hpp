#pragma once

#include "Output.hpp"

#include "vec.hpp"

#include <string>

void init(void);
void solve(void);

namespace aoc
{
  std::string &ProblemName();
  void register_problem(const char *name);
}