#pragma once
#include "src/config.hpp"
#include "src/erase_impl.hpp"

namespace tql
{
  // T tipini Typelistten siler (ilk bulduðu yerden siler)
  template <class Tlist, class T> 
  struct erase
  {
    typedef typename impl::erase_impl<Tlist, T>::result result;
    typedef result value_type;
  };

  // T tipini Typelistten siler (bütün liste içerisinde arar ve siler)
  template <class Tlist, class T> 
  struct erase_all
  {
    typedef typename impl::erase_all_impl<Tlist, T>::result result;
    typedef result value_type;
  };
}