// Advent of code 2024 - Day 07
// https://adventofcode.com/2024/day/7
// By: Arthur Baurens

#include <regex>
#include <cerrno>
#include <chrono>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <execution>
#include <functional>
#include <filesystem>

constexpr const char *const filepath = "assets/input.txt";

constexpr const char *const rxs = R"(([0-9]+):[ \t]+((?: ?[0-9]+)+))";

using num = uint64_t;
using num_list = std::vector<num>;
using equation = std::pair<num, num_list>;
using equation_list = std::vector<equation>;

using op = std::function<num(num, num)>;


static equation_list parse_input(const char *path)
{
  equation_list eqs;
  equation      eq;

  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  const std::regex rx(rxs);

  std::string line;
  std::smatch match;
  while (std::getline(ifs, line))
  {
    if (!std::regex_match(line, match, rx))
      throw "Invalid input line: " + line;

    eq.second.clear();

    eq.first = std::stoull(match[1]);
    std::stringstream ss(match[2]); 

    for (num operand; ss >> operand; )
      eq.second.emplace_back(std::move(operand));

    eqs.emplace_back(std::move(eq));
  }

  return eqs;
}

template<int N>
static num evaluate_equation(const equation &eq, uint32_t operators, num target = std::numeric_limits<num>::max())
{
  const op op_funcs[] = {
    /* add */ [](num a, num b) { return a + b; },
    /* sub */ [](num a, num b) { return a * b; },
    /* cat */ [](num a, num b) { for (num n = b; n /= 10;) a *= 10; return (a * 10) + b; }
  };

  num result = eq.second.front();
  for (size_t i = 1; i < eq.second.size(); ++i)
  {
    result = op_funcs[operators % N](result, eq.second[i]);
    operators /= N;
  }

  return result;
}

template<int N>
static bool check_equation(const equation &eq)
{
  uint32_t max;
  if constexpr (N == 2)
    max = 1 << (eq.second.size() - 1);
  else
    max = static_cast<uint32_t>(std::pow(N, eq.second.size() - 1));

  for (uint32_t operators = 0; operators < max; ++operators)
  {
    if (evaluate_equation<N>(eq, operators) == eq.first)
      return true;
  }
  return false;
}

static void solve_input(const char *path)
{
  const equation_list eqs = parse_input(path);

  std::vector<num> binaries(eqs.size(), 0);
  std::vector<num> ternaries(eqs.size(), 0);

  std::transform(
    std::execution::par,
    eqs.begin(),
    eqs.end(),
    binaries.begin(),
    [](const equation &eq) { return eq.first * check_equation<2>(eq); }
  );

  std::transform(
    std::execution::par,
    eqs.begin(),
    eqs.end(),
    ternaries.begin(),
    [](const equation &eq) { return eq.first * check_equation<3>(eq); }
  );

  const num binary  = std::reduce(std::execution::par, binaries.begin(),  binaries.end());
  const num ternary = std::reduce(std::execution::par, ternaries.begin(), ternaries.end());

  std::cout << "Sum of solvable (A): " << binary << std::endl;
  std::cout << "Sum of solvable (B): " << ternary << std::endl;
}

int main()
{
  using clock = std::chrono::high_resolution_clock;
  using ss = std::stringstream;

  try
  {
    const clock::time_point start_time = clock::now();
    solve_input(filepath);
    const clock::time_point end_time = clock::now();

    const uint64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    const std::string result = (ss() << "|  Problem solved in " << std::setprecision(3) << double(duration / 1000.0) << " seconds  |").str();
    const std::string border(result.size() - 2, '-');

    std::cout << '\n';
    std::cout << '+' << border << "+\n";
    std::cout << result << '\n';
    std::cout << '+' << border << '+' << std::endl;
  }
  catch (std::string &err)
  {
    std::cerr << "Error: " << err << std::endl;
    return 1;
  }

  return 0;
}

