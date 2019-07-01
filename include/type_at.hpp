/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "src/config.hpp"
#include "src/type_at_impl.hpp"

namespace tql
{
  template <class Tlist, unsigned int index> 
  struct type_at
  {
    typedef typename impl::type_at_impl<Tlist, index>::result result; 
    typedef result value_type;
  };
}