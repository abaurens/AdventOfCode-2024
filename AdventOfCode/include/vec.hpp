#pragma once

namespace aoc
{

  template<size_t Comp, class T>
  struct basic_vec {};

  template<class T>
  struct basic_vec<2, T>
  {
  private:
    constexpr static size_t Comp = 2;
    using type = basic_vec<Comp, T>;
    using bool_type = basic_vec<Comp, bool>;

    template<class TA, class TB>
    using add_rstp = decltype(TA(1) + TB(1));

  public:
    constexpr basic_vec(T x, T y) : x(x), y(y) {}

    template<class U>
    constexpr basic_vec(basic_vec<Comp, U> other) : basic_vec(other.x, other.y) {}
    constexpr basic_vec(T scalar) : basic_vec(scalar, scalar) {}
    constexpr basic_vec() : basic_vec(0) {}


    template<class U>
    constexpr operator basic_vec<Comp, U>() const { return basic_vec<Comp, U>{ x, y }; }

    // ==
    // !=
    template<class U>
    constexpr bool operator==(basic_vec<Comp, U> const &other) const
    {
      return x == other.x && y == other.y;
    }
    template<class U>
    constexpr bool operator!=(basic_vec<Comp, U> const &other) const
    {
      return x != other.x || y != other.y;
    }

    // <
    // >
    // <=
    // >=
    template<class U>
    constexpr bool_type operator<(basic_vec<Comp, U> const &other) const
    {
      return { x < other.x, y < other.y };
    }
    template<class U>
    constexpr bool_type operator>(basic_vec<Comp, U> const &other) const
    {
      return { x > other.x, y > other.y };
    }
    template<class U>
    constexpr bool_type operator<=(basic_vec<Comp, U> const &other) const
    {
      return { x < other.x, y < other.y };
    }
    template<class U>
    constexpr bool_type operator>=(basic_vec<Comp, U> const &other) const
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
    constexpr basic_vec<Comp, add_rstp<T, U>> operator+(basic_vec<Comp, U> t) const { return {x + t.x, y + t.y}; }
    template<class U>
    constexpr basic_vec<Comp, add_rstp<T, U>> operator-(basic_vec<Comp, U> t) const { return { x - t.x, y - t.y }; }
    template<class U>
    constexpr basic_vec<Comp, add_rstp<T, U>> operator*(basic_vec<Comp, U> t) const { return { x * t.x, y * t.y }; }
    template<class U>
    constexpr basic_vec<Comp, add_rstp<T, U>> operator/(basic_vec<Comp, U> t) const { return { x / t.x, y / t.y }; }

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
    constexpr type &operator+=(basic_vec<Comp, U> t) { x += t.x; y += t.y; return *this; }
    template<class U>
    constexpr type &operator-=(basic_vec<Comp, U> t) { x -= t.x; y -= t.y; return *this; }
    template<class U>
    constexpr type &operator*=(basic_vec<Comp, U> t) { x *= t.x; y *= t.y; return *this; }
    template<class U>
    constexpr type &operator/=(basic_vec<Comp, U> t) { x /= t.x; y /= t.y; return *this; }

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
    T x;
    T y;
  };

  using vec2  = basic_vec<2, int>;
  using vec2b = basic_vec<2, bool>;

}