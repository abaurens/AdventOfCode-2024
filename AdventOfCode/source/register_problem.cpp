#pragma once

#include <string>

namespace aoc
{
  std::string &ProblemName()
  {
    static std::string PROBLEM_NAME = "";

    return PROBLEM_NAME;
  }

  void register_problem(const char *name)
  {
    ProblemName() = name;
  }

}