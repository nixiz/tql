/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "../src/config.hpp"
#include "../src/distinct_impl.hpp"

namespace tql
{
  // Removes duplicates from the list
  template <class Tlist> 
  struct distinct
  {
    typedef typename impl::distinct_impl<Tlist>::result result;
    typedef result value_type;
  };
}