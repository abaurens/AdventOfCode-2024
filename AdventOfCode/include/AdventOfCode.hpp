#pragma once

#include "types.hpp"
#include "output.hpp"

#include "rect_map.hpp"
#include "cvector.hpp"
#include "vec.hpp"

#include <cerrno>
#include <cassert>
#include <cstdlib>

#include <set>
#include <map>
#include <list>
#include <tuple>
#include <array>
#include <deque>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string_view>

#include <limits>
#include <iomanip>
#include <numeric>
#include <execution>
#include <algorithm>

void init(void);
void solve(void);

namespace aoc
{
  std::string &ProblemName();
  void register_problem(const char *name);

  using u8  = uint8_t;
  using u16 = uint16_t;
  using u32 = uint32_t;
  using u64 = uint64_t;
  
  using i8  = int8_t;
  using i16 = int16_t;
  using i32 = int32_t;
  using i64 = int64_t;
}

using aoc::u8;
using aoc::u16;
using aoc::u32;
using aoc::u64;

using aoc::i8;
using aoc::i16;
using aoc::i32;
using aoc::i64;

using aoc::uint;
