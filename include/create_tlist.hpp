/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "../src/config.hpp"
#include "../src/create_tlist_impl.hpp"

namespace tql
{
  template <class ...Elems>
  struct create_tlist
  {
    using result = typename impl::create_tlist_impl<tql::empty_list, Elems...>::result;
  };
  
  template <class ...Elems>
  using create_tlist_t = typename create_tlist<Elems...>::result;
}