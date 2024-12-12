#pragma once

#include "types.hpp"

#include <cassert>

namespace aoc
{
  template<class T, size_t Capacity>
  class cvector
  {
  public:
    using iterator = T*;
    using const_iterator = const T*;

  public:
    constexpr cvector() = default;

    constexpr cvector(const cvector &other)
    {
      for (size_t i = 0; i < other.m_size; ++i)
        new (data() + i) T(other.data()[i]);
      m_size = other.m_size;
    }

    constexpr cvector(cvector &&other)
    {
      m_size = other.m_size;
      std::memcpy(m_data, other.m_data, m_size * sizeof(T));
      other.m_size = 0;
    }

    constexpr cvector(const std::initializer_list<T> &list)
    {
      assert(list.size() <= Capacity);

      for (const T &value : list)
        push_back(value);
    }


    constexpr size_t capacity() const { return Capacity; }
    constexpr size_t size() const { return m_size; }
    constexpr bool empty() const { return m_size == 0; }

    constexpr T *data() { return static_cast<T*>(m_data); }
    constexpr const T *data() const { return static_cast<const T*>(m_data); }

    void clear()
    {
      for (size_t i = 0; i < m_size; ++i)
        m_data[i].~T();
      m_size = 0;
    }

    void push_back(const T &value)
    {
      assert(m_size < Capacity);

      if (m_size < Capacity)
        new (m_data + m_size++) T(value);
    }

    template<class... Args>
    void emplace_back(Args &&...args)
    {
      assert(m_size < Capacity);

      if (m_size < Capacity)
        new (data() + m_size++) T(std::forward<Args>(args)...);
    }

    iterator erase(iterator pos)
    {
      assert(pos >= begin() && pos < end());

      if (pos < end())
      {
        std::memmove(pos, pos + 1, (end() - pos - 1) * sizeof(T));
        --m_size;
      }
      return pos;
    }

    constexpr T &operator[](size_t index) { return data()[index]; }
    constexpr const T &operator[](size_t index) const { return data()[index]; }

    iterator begin() { return data(); }
    const_iterator begin() const { return data(); }

    iterator end() { return data() + m_size; }
    const_iterator end() const { return data() + m_size; }

  private:
    u8 m_data[Capacity * sizeof(T)];
    size_t m_size = 0;
  };

}