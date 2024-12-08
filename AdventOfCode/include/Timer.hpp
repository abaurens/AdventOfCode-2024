#pragma once

#include "concepts.hpp"

#include <ratio>
#include <chrono>
#include <cstdint>
#include <concepts>

namespace aoc
{
  template <class _Ty>
  constexpr bool is_ratio_v = false; // test for ratio type

  template <intmax_t _Rx1, intmax_t _Rx2>
  constexpr bool is_ratio_v<std::ratio<_Rx1, _Rx2>> = true;



  template<class T>
  constexpr bool is_duration_v = is_specialization_v<T, std::chrono::duration>;

  class Timer
  {
    using clock = std::chrono::high_resolution_clock;


  public:
    Timer() : start_time(clock::now()) {}

    void reset() { start_time = clock::now(); }

    template<class Dur>
    requires is_duration_v<Dur>
    Dur::rep GetTime() const
    {
      return std::chrono::duration_cast<Dur>(clock::now() - start_time).count();
    }

    template<class Rep, class Period = std::ratio<1, 1>>
      requires (std::integral<Rep> || std::floating_point<Rep>) && is_ratio_v<Period>
    Rep GetTime() const
    {
    
      return std::chrono::duration_cast<std::chrono::duration<Rep, Period>>(clock::now() - start_time).count();
    }

  private:
    clock::time_point start_time;
  };

}