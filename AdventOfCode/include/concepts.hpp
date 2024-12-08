#pragma once

namespace aoc
{
  // true if and only if T is a specialization of Template
  template <class T, template <class...> class Template>
  constexpr bool is_specialization_v = false;

  template <template <class...> class Template, class... Ts>
  constexpr bool is_specialization_v<Template<Ts...>, Template> = true;

}