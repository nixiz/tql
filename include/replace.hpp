#pragma once
#include "src/config.hpp"
#include "src/replace_impl.hpp"

namespace tql
{
  // Removes duplicates from the list
  template <class Tlist, class T, class U> 
  struct replace_first
  {
    typedef typename impl::replace_impl<Tlist, T, U>::result result;
    typedef result value_type;
  };

  template <class Tlist, class T, class U> 
  struct replace_all
  {
    // typedef typename impl::replace_impl<Tlist, T, U>::result result;
    // typedef result value_type;
  };
}