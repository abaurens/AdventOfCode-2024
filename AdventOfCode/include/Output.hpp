#pragma once

#include <vector>
#include <sstream>
#include <iostream>

namespace aoc
{
  class output
  {
    using Stream = std::stringstream;
    using LineList = std::vector<std::string>;

  public:
    void put(char c)
    {
      m_ss.put(c);
    }

    void clear()
    {
      m_ss.clear();
      m_lines.clear();
    }

    void flush()
    {
      if (m_ss.view().size() == 0)
        return;

      std::string line;
      while (std::getline(m_ss, line))
      {
        if (line.size() > m_width)
          m_width = line.size();

        m_lines.emplace_back(std::move(line));
        line.clear();
      }
      m_ss.clear();
    }

    size_t width() const { return m_width; }
    size_t height() const { return m_lines.size(); }

    LineList::iterator begin() { return m_lines.begin(); }
    LineList::iterator end()   { return m_lines.end(); }

    LineList::const_iterator begin() const { return m_lines.begin(); }
    LineList::const_iterator end() const { return m_lines.end(); }

    operator bool() const
    {
      return (!m_lines.empty() && m_ss.view().size() != 0);
    }

  public:
    template<class T>
    friend inline output &operator<<(output &s, T output)
    {
      s.m_ss << output;
      return s;
    }

    friend inline output &operator<<(output &s, const char *str)
    {
      s.m_ss << str;
      return s;
    }

    friend inline output &operator<<(output &s, output &(*pf)(output &))
    {
      return pf(s);
    }

  private:
    Stream   m_ss;
    size_t   m_width = 0;
    LineList m_lines;
  };

  extern "C++" output cout;
}

namespace std
{
  // insert newline and flush stream
  inline ::aoc::output &endl(::aoc::output &out)
  {
    out.put('\n');
    out.flush();
    return out;
  }
}