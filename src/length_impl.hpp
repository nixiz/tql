#pragma once
#include "include/type_list.hpp"

namespace tql
{
namespace impl
{
    template <class Tlist> struct length_impl;
    
    template <> struct length_impl<null_type>
    {
      static const int value = 0;
    };

    template <> struct length_impl<empty_list>
    {
      static const int value = 0;
    };

    template <class T, class U>
    struct length_impl<type_list<T, U> >
    {
      static const int value = 1 + length_impl<U>::value;
    };

} // namespace impl.
} // namespace tql.