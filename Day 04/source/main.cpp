// Advent of code 2024 - Day 04
// https://adventofcode.com/2024/day/4
// By: Arthur Baurens

#include <cerrno>
#include <fstream>
#include <iostream>
#include <filesystem>

constexpr const char *const filepath = "assets/input.txt";
constexpr const char *const search   = "XMAS";
constexpr const char *const x_search = search + 1;

constexpr int read_size = 4096;

#define BIT(x) (1 << x)

using Dir = int;

constexpr Dir UP    = BIT(0);
constexpr Dir DOWN  = BIT(1);
constexpr Dir RIGHT = BIT(2);
constexpr Dir LEFT  = BIT(3);

struct Input
{
  size_t width = 0;
  size_t height = 0;
  std::string data;
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

static Input load_file(const char *path)
{
  if (!std::filesystem::exists(path))
    throw "Input file " + std::string(path) + " does not exist";

  std::ifstream ifs(path);

  if (!ifs.is_open())
    throw "Can't open input file '" + std::string(filepath) + "': " + strerror(errno);

  Input result;

  result.height = !ifs.eof(); // line count start from 1, but only if the file isn't empty

  size_t count = 0;
  char buffer[read_size + 1];
  while (!ifs.eof())
  {
    count = read(ifs, buffer, read_size);
    buffer[count] = '\0';

    const char *line_start = buffer;
    const char *line_end;

    while (*(line_end = strchrnul(line_start, '\n')))
    {

      result.data.append(line_start, line_end - line_start);
      line_start = line_end + 1;

      if (result.width == 0)
        result.width = result.data.size();

      if (!ifs.eof() || line_end[1]) // only add line if there is more data to be treated
        result.height++;
    }

    result.data.append(line_start, line_end - line_start);
  }

  return result;
}

template<Dir dir>
static int dir_to_offset(const Input &input)
{
  int result = 0;

  if constexpr (dir & RIGHT)
    result += 1;
  if constexpr (dir & LEFT)
    result -= 1;
  if constexpr (dir & DOWN)
    result += (int)input.width;
  if constexpr (dir & UP)
    result -= (int)input.width;
  return result;
}

template<Dir dir>
static bool search_str_dir(const Input &input, int pos, const char *const needle)
{
  const size_t length = strlen(needle) - 1;
  const int x_start = int(pos % input.width);
  const int y_start = int(pos / input.width);
  const int x_end = x_start + (!!(dir & RIGHT) - !!(dir & LEFT)) * int(length);
  const int y_end = y_start + (!!(dir & DOWN)  - !!(dir & UP))   * int(length);

  if (x_end < 0 || x_end >= input.width)
    return false;
  if (y_end < 0 || y_end >= input.height)
    return false;

  const int dir_offset = dir_to_offset<dir>(input);
  for (int i = 0; needle[i]; ++i)
  {
    if (pos < 0 || pos >= input.data.size() || input.data[pos] != needle[i])
      return false;

    pos += dir_offset;
  }
  return true;
}
#define chr search_str_dir

static size_t search_str(const Input &input, int pos, const char *const needle)
{
  size_t result = 0;

  if (!*needle || input.data[pos] != *needle)
    return 0;

  if (!needle[1]) // if the needle is 1 character long, this is the whole match
    return 1;

  result += search_str_dir<RIGHT>(input, pos, needle);
  result += search_str_dir<DOWN >(input, pos, needle);
  result += search_str_dir<LEFT >(input, pos, needle);
  result += search_str_dir<UP   >(input, pos, needle);

  result += search_str_dir<RIGHT | DOWN>(input, pos, needle);
  result += search_str_dir<RIGHT | UP  >(input, pos, needle);
  result += search_str_dir<LEFT  | DOWN>(input, pos, needle);
  result += search_str_dir<LEFT  | UP  >(input, pos, needle);
  return result;
}

static size_t search_X(const Input &input, int pos, const char *const needle)
{
  bool result = true;

  const size_t len = strlen(needle);
  const int half_len = int(len / 2);

  if (input.data[pos] != needle[len / 2])
    return 0;

  if (len == 1) // if the needle is 1 character long, this is the whole match
    return 1;

  if (len % 2 == 0)
    throw "Needle size must be odd";

  const int x = int(pos % input.width);
  const int y = int(pos / input.width);
  const int box[4] = {
    (x - half_len), (y - half_len),
    (x + half_len), (y + half_len)
  };

  // if the X does not fit in that place
  if (box[0] < 0 || box[1] < 0 || box[2] >= input.width || box[3] >= input.height)
    return 0;

  // (x, y) -> i
  auto toi = [&input](int x, int y) { return int(x + y * input.width); };

  // There is 4 possible combination (2 per diagonal)

  // check first diagonal
  result = (
       chr<RIGHT | DOWN>(input, toi(x - half_len, y - half_len), needle)
    || chr<LEFT  | UP  >(input, toi(x + half_len, y + half_len), needle)
  );

  // check second diagonal
  result &= (
       chr<RIGHT | UP  >(input, toi(x - half_len, y + half_len), needle)
    || chr<LEFT  | DOWN>(input, toi(x + half_len, y - half_len), needle)
  );

  return result;
}

static void solve_input(const char *path)
{
  Input input = load_file(path);

  size_t xmas = 0;
  size_t x_mas = 0;
  for (int i = 0; i < input.data.size(); ++i)
  {
    xmas  += search_str(input, i, search);
    x_mas += search_X(input, i, x_search);
  }
  std::cout << "Found '" << search << "' " << xmas << " times\n";
  std::cout << "Found " << x_mas << " X-" << x_search << "es\n";
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
