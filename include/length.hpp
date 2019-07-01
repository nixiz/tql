/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "src/config.hpp"
#include "src/length_impl.hpp"

namespace tql
{
  template <class Tlist> struct length
  {
    static constexpr int value = impl::length_impl<Tlist>::value; 
    typedef decltype(value) value_type;
  };
}