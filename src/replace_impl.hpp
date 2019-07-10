/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "../include/type_list.hpp"

namespace tql
{
namespace impl
{
  	template <class Tlist, class T, class U> struct replace_impl;

    template <class T, class U> 
    struct replace_impl<null_type, T, U>
    {
      typedef null_type result;
    };

    template <class T, class Tail, class U>
    struct replace_impl<type_list<T, Tail>, T, U>
    {
      typedef type_list<U, Tail> result;
    };

    template <class Head, class Tail, class T, class U>
    struct replace_impl<type_list<Head, Tail>, T, U>
    {
      typedef type_list<Head, typename replace_impl<Tail, T, U>::result> result;
    };
} // namespace impl.
} // namespace tql.