#pragma once
#include "src/config.hpp"
#include "src/contains_impl.hpp"

namespace tql
{
  template <class TList, class T> 
  struct contains
  {
    static constexpr bool value = impl::contains_impl<TList, T>::value;
    typedef decltype(value) value_type;
  };
}