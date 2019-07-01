#pragma once
#include "src/config.hpp"
#include "src/append_impl.hpp"

namespace tql
{
  template <class Tlist, class T>
  struct append
  {
    typedef typename impl::append_impl<Tlist, T>::result result;
    typedef result value_type;
  };
}