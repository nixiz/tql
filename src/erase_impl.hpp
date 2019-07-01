#pragma once
#include "include/type_list.hpp"

namespace tql
{
namespace impl
{
    template <class Tlist, class T> struct erase_impl;
    template <class Tlist, class T> struct erase_all_impl;

    template <class T> 
    struct erase_impl<null_type, T>
    {
      typedef null_type result;
    };

    template <class T, class Tail>
    struct erase_impl<type_list<T, Tail>, T>	// eðer T tipi typelist içerisindeki Head ise o zaman sonuç head dýþýnda kalan Tail'dir
    {
      typedef Tail result;
    };

    template <class Head, class Tail, class T>
    struct erase_impl<type_list<Head, Tail>, T>	// T tipi type_list içerisinde yoksa, list üzerinde itere ederek aramaya devam eder
    {
      typedef type_list<Head, typename erase_impl<Tail, T>::result> result;
    };

    // ---- erase all ---- //

    template <class T>
    struct erase_all_impl<null_type, T>
    {
      typedef null_type result;
    };

    template <class T, class Tail>
    struct erase_all_impl<type_list<T, Tail>, T>
    {
      // get all the way down the list removing the type
      typedef typename erase_all_impl<Tail, T>::result result;
    };

    template <class Head, class Tail, class T>
    struct erase_all_impl<type_list<Head, Tail>, T>
    {
      // go all the way down the list removing the type
      typedef type_list<Head, typename erase_all_impl<Tail, T>::result> result;
    };
} // namespace impl.
} // namespace tql.