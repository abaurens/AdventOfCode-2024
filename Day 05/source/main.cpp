// Advent of code 2024 - Day 05
// https://adventofcode.com/2024/day/5
// By: Arthur Baurens

#include <set>
#include <map>
#include <regex>
#include <chrono>
#include <cerrno>
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

constexpr const char *const filepath = "assets/input.txt";

constexpr const char *const rule_rxs = R"(^([0-9]+)\|([0-9]+)$)";
constexpr const char *const update_rxs = R"(,?([0-9]+))";

// a rule is defined by it's page number and the pages that it precedes
using ruleset = std::set<int>;

struct page
{
  int weight;
  ruleset rules;
};

struct Update
{
  std::map<int, page> pages;
  std::vector<int>    pages_order;
};

static std::vector<ruleset> parse_rules(std::istream &is)
{
  std::vector<ruleset> result;

  std::string line;
  std::smatch match;
  const std::regex rule_rx(rule_rxs);

  while (std::getline(is, line) && std::regex_match(line, match, rule_rx))
  {
    int X = std::stoi(match[1]);
    int Y = std::stoi(match[2]);

    int max = std::max(X, Y);
    if (result.size() <= max)
      result.resize(max + 1);

    // register that page Y goes after page X
    result[X].insert(Y);
  }

  assert(line.size() == 0);
  return result;
}

std::vector<Update> parse_updates(std::istream &is, std::vector<ruleset> rules)
{
  std::vector<Update> result;

  std::string line;
  std::smatch match;
  const std::regex update_rx(update_rxs);

  while (std::getline(is, line))
  {
    if (line.empty())
      continue;

    Update update;

    while (std::regex_search(line, match, update_rx))
    {
      const int page_num = std::stoi(match[1]);

      update.pages[page_num] = { 0, rules[page_num] }; // copy assign the ruleset for the page
      update.pages_order.emplace_back(page_num);

      line = match.suffix();
    }

    // remove all useless rules (the ones that are not used in this update)
    for (auto &[page_id, _] : update.pages)
    {
      std::erase_if(
        update.pages[page_id].rules,
        [&update](int p) { return !update.pages.contains(p); }
      );
    }
    result.emplace_back(std::move(update));
  }

  return result;
}

static std::vector<Update> parse_input(const char *path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  std::vector<ruleset> rules = parse_rules(ifs);

  return parse_updates(ifs, rules);
}

// update the page weight and recursively propagate the change
static void set_page_weight(Update &update, page &page, int weight)
{
  if (page.weight >= weight)
    return;

  page.weight = weight;
  for (int p : page.rules)
    set_page_weight(update, update.pages[p], page.weight + 1);
}

static void pre_process_update(Update &update)
{
  for (auto &[page_id, page] : update.pages)
  {
    for (int p : page.rules)
      set_page_weight(update, page, update.pages[p].weight + 1);
  }
}

static void solve_input(const char *path)
{
  std::vector<Update> updates = parse_input(path);

  int middle_sorted_sum = 0;
  int middle_unsorted_sum = 0;

  for (Update &update : updates)
  {
    pre_process_update(update);

    const bool sorted = std::is_sorted(
      update.pages_order.begin(),
      update.pages_order.end(),
      [&update](int a, int b) { return (update.pages[a].weight < update.pages[b].weight); }
    );

    if (sorted)
    {
      middle_sorted_sum += update.pages_order[update.pages_order.size() / 2];
      //std::cout << "Sorted:";
      //for (int p : update.pages_order)
      //  std::cout << ' ' << p;
      //std::cout << std::endl;
    }
    else
    {
      //std::cout << "Unsorted:";
      //for (int p : update.pages_order)
      //  std::cout << ' ' << p;
      //std::cout << std::endl;

      std::sort(
        update.pages_order.begin(),
        update.pages_order.end(),
        [&update](int a, int b) { return (update.pages[a].weight < update.pages[b].weight); }
      );
      middle_unsorted_sum += update.pages_order[update.pages_order.size() / 2];
    }
  }

  std::cout << std::endl;
  std::cout << "Sum of sorted middle pages: " << middle_sorted_sum << std::endl;
  std::cout << "Sum of unsorted middle pages: " << middle_unsorted_sum << std::endl;
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
