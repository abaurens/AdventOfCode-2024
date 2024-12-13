#pragma once

#include "types.hpp"

namespace aoc
{
  template<size_t Comp, class T>
  struct vec {};

  template<class T>
  struct vec<2, T>
  {
  private:
    constexpr static size_t Comp = 2;
    using type = vec<Comp, T>;
    using bool_type = vec<Comp, bool>;

    template<class TA, class TB>
    using add_rstp = decltype(TA(1) + TB(1)); // yeilds the result type of TA + TB

  public:
    constexpr vec(T x, T y) : x(x), y(y) {}

    template<class U>
    constexpr vec(vec<Comp, U> other) : vec(other.x, other.y) {}
    constexpr vec(T scalar) : vec(scalar, scalar) {}
    constexpr vec() : vec(0) {}


    template<class U>
    constexpr operator vec<Comp, U>() const { return vec<Comp, U>{ x, y }; }

    // ==
    // !=
    template<class U>
    constexpr bool operator==(vec<Comp, U> const &other) const
    {
      return x == other.x && y == other.y;
    }
    template<class U>
    constexpr bool operator!=(vec<Comp, U> const &other) const
    {
      return x != other.x || y != other.y;
    }

    // <
    // >
    // <=
    // >=
    template<class U>
    constexpr bool_type operator<(vec<Comp, U> const &other) const
    {
      return { x < other.x, y < other.y };
    }
    template<class U>
    constexpr bool_type operator>(vec<Comp, U> const &other) const
    {
      return { x > other.x, y > other.y };
    }
    template<class U>
    constexpr bool_type operator<=(vec<Comp, U> const &other) const
    {
      return { x < other.x, y < other.y };
    }
    template<class U>
    constexpr bool_type operator>=(vec<Comp, U> const &other) const
    {
      return { x >= other.x, y >= other.y };
    }

    // +
    // -
    // *
    // /
    template<class U>
    constexpr type operator+(U scalar) const { return { x + scalar, y + scalar }; }
    template<class U>
    constexpr type operator-(U scalar) const { return { x - scalar, y - scalar }; }
    template<class U>
    constexpr type operator*(U scalar) const { return { x * scalar, y * scalar }; }
    template<class U>
    constexpr type operator/(U scalar) const { return { x / scalar, y / scalar }; }

    template<class U>
    constexpr vec<Comp, add_rstp<T, U>> operator+(vec<Comp, U> t) const { return {x + t.x, y + t.y}; }
    template<class U>
    constexpr vec<Comp, add_rstp<T, U>> operator-(vec<Comp, U> t) const { return { x - t.x, y - t.y }; }
    template<class U>
    constexpr vec<Comp, add_rstp<T, U>> operator*(vec<Comp, U> t) const { return { x * t.x, y * t.y }; }
    template<class U>
    constexpr vec<Comp, add_rstp<T, U>> operator/(vec<Comp, U> t) const { return { x / t.x, y / t.y }; }

    // +=
    // -=
    // *=
    // /=
    template<class U>
    constexpr type &operator+=(U scalar) { x += scalar; y += scalar; return *this; }
    template<class U>
    constexpr type &operator-=(U scalar) { x -= scalar; y -= scalar; return *this; }
    template<class U>
    constexpr type &operator*=(U scalar) { x *= scalar; y *= scalar; return *this; }
    template<class U>
    constexpr type &operator/=(U scalar) { x /= scalar; y /= scalar; return *this; }

    template<class U>
    constexpr type &operator+=(vec<Comp, U> t) { x += t.x; y += t.y; return *this; }
    template<class U>
    constexpr type &operator-=(vec<Comp, U> t) { x -= t.x; y -= t.y; return *this; }
    template<class U>
    constexpr type &operator*=(vec<Comp, U> t) { x *= t.x; y *= t.y; return *this; }
    template<class U>
    constexpr type &operator/=(vec<Comp, U> t) { x /= t.x; y /= t.y; return *this; }

    // []
    constexpr T &operator[](int i)
    {
      switch (i)
      {
      default:
      case 0:
        return x;
      case 1:
        return y;
      }
    }

    constexpr T operator[](int i) const
    {
      switch (i)
      {
      default:
      case 0:
        return x;
      case 1:
        return y;
      }
    }

  public:
    union { T x, w; };
    union { T y, h; };
  };

  using vec2  = vec<2, int>;
  using vec2u = vec<2, uint>;
  using vec2b = vec<2, bool>;
  using vec2s = vec<2, size_t>;
}

template <class T>
struct std::less<aoc::vec<2, T>>
{
  constexpr bool operator()(const aoc::vec<2, T> &a, const aoc::vec<2, T> &b) const
  {
    const aoc::vec2b low = (a < b);
    return low.x || (a.x == b.x && low.y);
  }
};

template<class T>
struct std::hash<aoc::vec<2, T>>
{
  constexpr size_t operator()(const aoc::vec<2, T> &value) const
  {
    constexpr std::hash<T> hasher;

    size_t hash = 0;
    hash ^= hasher(value.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= hasher(value.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return hash;
  }
};