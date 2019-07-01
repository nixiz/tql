/*
 Copyright (c) Oguzhan KATLI
 Licensed under the MIT license.
*/
#pragma once
#include "include/type_list.hpp"

namespace tql
{
namespace impl
{
    template <class Tlist, class T> struct append_impl;
    // Tlist'e T tipini ekler
    // template <class Tlist, class T> struct append;
    // özelleştirme : null tipler için
    template <>
    struct append_impl<null_type, null_type>
    {
      typedef null_type result;
    };
    // özelleştirme (1)
    template <class T>
    struct append_impl<null_type, T>
    {
      typedef type_list<T, null_type> result;
    };

    // özelleştirme (2)
    template <class T>
    struct append_impl<type_list<null_type, null_type>, T>
    {
      typedef type_list<T, null_type> result;
    };

    // özelleştirme (3)
    template <class Head, class Tail>
    struct append_impl<null_type, type_list<Head, Tail> >
    {
      typedef type_list<Head, Tail> result;
    };

    // özelleştirme (4)
    template <class Head, class Tail, class T>
    struct append_impl<type_list<Head, Tail>, T>
    {
      typedef type_list<Head, typename append_impl<Tail, T>::result> result;
    };

} // namespace impl.
} // namespace tql.