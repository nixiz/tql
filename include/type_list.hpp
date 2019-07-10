/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "../src/config.hpp"

namespace tql
{

  template <class T, class U = null_type>
  struct type_list
  {
    typedef T Head;
    typedef U Tail;
  };
  // empty list 
  using empty_list = type_list<null_type, null_type>;
} // namespace tql