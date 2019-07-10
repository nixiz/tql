/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "../src/config.hpp"
#include "../src/index_of_impl.hpp"

namespace tql
{
  template <class TList, class T> 
  struct index_of
  {
    static constexpr int value = impl::index_of_impl<TList, T>::value;
    typedef decltype(value) value_type;
  };
}