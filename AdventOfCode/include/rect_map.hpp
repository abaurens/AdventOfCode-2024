#pragma once

#include "vec.hpp"

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

namespace aoc
{
  template<class T>
  struct loader
  {
    constexpr T operator()(char c) { return T(c); };
  };

  template<class T = i8, class Loader=loader<T>>
  class rect_map
  {
  public:
    rect_map() = default;

    vec2s size() const { return m_size; }
    size_t width() const { return m_size.w; }
    size_t height() const { return m_size.h; }

    template<class U>
      requires std::is_integral_v<U>
    T &operator[](vec<2, U> pos) { return m_data[pos.y * m_size.w + pos.x]; }
    template<class U>
      requires std::is_integral_v<U>
    const T &operator[](vec<2, U> pos) const { return m_data[pos.y * m_size.w + pos.x]; }

    template<class U>
      requires std::is_integral_v<U>
    bool contains(vec<2, U> pos) const { return pos.x >= 0 && pos.x < m_size.w && pos.y >= 0 && pos.y < m_size.h; }

    void reserve(size_t size) { m_data.reserve(size); }

    friend std::istream &operator>>(std::istream &is, rect_map &map)
    {
      std::string line;

      map.m_size = { 0, 0 };
      while (std::getline(is, line))
      {
        if (map.m_size.w)
          assert(line.size() == map.m_size.w);
        else
          map.m_size.w = line.size();

        for (auto &c : line)
          map.m_data.push_back(Loader()(c));
        //map.m_data.insert(map.data.end(), line.begin(), line.end());

        ++map.m_size.h;
      }
      return is;
    }


  private:
    vec2s m_size = { 0, 0 };
    std::vector<T> m_data;
  };
}