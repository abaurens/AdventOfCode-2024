// Advent of code 2024 - Day 01
// https://adventofcode.com/2024/day/1
// By: Arthur Baurens

#include <cerrno>
#include <cassert>

#include <vector>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

using elem_type = int;
using list_type = std::vector<elem_type>;

using path = std::filesystem::path;

// Binary search trough the sorted list `lst` to find `elem`.
//   If `elem` is not present, returns the index of the first element greater than `elem`.
//   If present, returns the index of an occurence of `elem` (not necessarily the first one).
static size_t binary_search(const list_type &lst, const elem_type &elem)
{
  size_t begin = 0;
  size_t end = lst.size();
  size_t mid = 0;

  while (begin != end)
  {
    mid = (end - begin) / 2 + begin;

    if (elem < lst[mid])
      end = mid;
    else if (elem > lst[mid])
      begin = ++mid;
    else
      break;
  }
  return mid;
}

// The list is sorted. so counting an element occurence is trivial
static size_t count_occurrences(const list_type &lst, elem_type elem)
{
  size_t i = binary_search(lst, elem);

  if (lst[i] != elem)
    return 0;

  size_t count = 1;
  while (i > 0 && lst[i - 1] == elem)
  {
    ++count;
    --i;
  }
  while (i + count < lst.size() && lst[i + count] == elem)
    ++count;

  return count;
}

// Read the input file and insert the elements in the lists in sorted order
static void load_lists(path filepath, list_type &list_a, list_type &list_b)
{
  if (!std::filesystem::exists(filepath))
    throw "Input file " + filepath.string() + " does not exist";

  std::ifstream ifs(filepath);

  if (!ifs.is_open())
    throw "Can't open input file '" + filepath.string() + "': " + strerror(errno);

  elem_type a;
  elem_type b;
  while (ifs >> a >> b)
  {
    // insert elements in sorted position
    list_a.emplace(
      list_a.begin() + binary_search(list_a, a),
      std::move(a)
    );
    list_b.emplace(
      list_b.begin() + binary_search(list_b, b),
      std::move(b)
    );
  }

  assert(list_a.size() == list_b.size());
  assert(std::is_sorted(list_a.begin(), list_a.end()));
  assert(std::is_sorted(list_b.begin(), list_b.end()));
}

int main()
{
  list_type list_a;
  list_type list_b;
  list_type results;

  load_lists("assets/input.txt", list_a, list_b);
  results.resize(list_a.size());

  // Compute distance
  std::transform(
    list_a.begin(), list_a.end(),
    list_b.begin(),
    results.begin(),
    [](elem_type &a, elem_type &b) { return std::abs(a - b); }
  );
  const int distance = std::reduce(results.cbegin(), results.cend());

  // Compute similarity
  std::transform(
    list_a.begin(), list_a.end(),
    results.begin(),
    [&list_b](elem_type &a) { return a * (int)count_occurrences(list_b, a); }
  );
  const int similarity = std::reduce(results.cbegin(), results.cend());

  // Display results
  std::cout << "Distance: " << distance << std::endl;
  std::cout << "Similarity: " << similarity << std::endl;
}
