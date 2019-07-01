/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "include/type_list.hpp"
#include "include/iterators.hpp"

namespace tql
{
namespace impl
{
    // template tag dispatch helpers
    struct use_from_predicate {};
    struct use_to_predicate {};

    template <class tag_dispatch, class Tlist, class _From, class Iter,
      template <class, class> class predicate>
    struct query_impl;


    template <class Tlist, class _To, class Iter,
      template <class, class> class predicate>
    struct query_impl<use_to_predicate, Tlist, _To, Iter, predicate>
    {
    public:
      static const bool value = predicate<typename Iter::value, _To>::value == true ? true : query_impl<use_to_predicate, Tlist, _To, typename next<Tlist, Iter>::iter, predicate>::value;
    };

    template <class Tlist, class _From, class Iter,
      template <class, class> class predicate>
    struct query_impl<use_from_predicate, Tlist, _From, Iter, predicate>
    {
    public:
      static const bool value = predicate<_From, typename Iter::value>::value == true ? true : query_impl<use_from_predicate, Tlist, _From, typename next<Tlist, Iter>::iter, predicate>::value;
    };

    template <class Tlist, class T, template <class, class> class predicate>
    struct query_impl<use_to_predicate, Tlist, T, null_type, predicate>
    {
    public:
      static const bool value = false;
    };

    template <class Tlist, class _From, unsigned int index, template <class, class> class predicate>
    struct query_impl<use_from_predicate, Tlist, _From, impl::iterator<null_type, index>, predicate>
    {
    public:
      static const bool value = false;
    };

    template <class Tlist, class _From, unsigned int index, template <class, class> class predicate>
    struct query_impl<use_to_predicate, Tlist, _From, impl::iterator<null_type, index>, predicate>
    {
    public:
      static const bool value = false;
    };    
} // namespace impl.
} // namespace tql.