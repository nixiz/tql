/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "../include/type_list.hpp"
#include "../include/erase.hpp"

namespace tql
{
namespace impl
{
  	template <class Tlist> struct distinct_impl;

    template <> 
    struct distinct_impl<null_type>
    {
      typedef null_type result;
    };

    template <>
    struct distinct_impl<empty_list>
    {
      typedef null_type result;
    };

    /*
    * If TList is null_type, then result is null_type.
    * Else
    *   Apply NoDuplicates to TList::Tail, obtaining a temporary typelist L1.
    *   Apply erase to L1 and TList::Head. Obtain L2 as the result.
    *   result is a typelist whose head is TList::Head and whose tail is L2.
    */
    template <class Head, class Tail>
    struct distinct_impl<type_list<Head, Tail>>
    {
    private:
      typedef typename distinct_impl<Tail>::result L1;
      typedef typename erase<L1, Head>::result L2;
    public:
      typedef type_list<Head, L2> result;
    };
} // namespace impl.
} // namespace tql.