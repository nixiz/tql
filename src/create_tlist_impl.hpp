#pragma once
#include "include/type_list.hpp"
#include "include/append.hpp"

namespace tql
{
/* Create list helpers for c++11 and higher standards*/
namespace impl
{
    // generic tpyelist creator decleration
    template <typename Tlist, typename ...Elems>
    struct create_tlist_impl;

    template <typename Tlist>
    struct create_tlist_impl<Tlist>
    {
        using result = Tlist;
    };

    template <typename Tlist, typename Elem>
    struct create_tlist_impl<Tlist, Elem>
    {
        using result = typename tql::append<Tlist, Elem>::result;
    };

    template <typename Tlist, typename Elem, typename ...Others>
    struct create_tlist_impl<Tlist, Elem, Others...>
    {
        typedef typename tql::append<Tlist, Elem>::result tlist;
        using result = typename create_tlist_impl<tlist, Others...>::result;
    };
} // namespace impl.
} // namespace tql.