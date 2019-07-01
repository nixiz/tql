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
    template <class TList, class T>  struct contains_impl;

    template <class TList, class T> 
    struct contains_impl
    {
      static const bool value = false;
    };

    // listede eleman yoksa
    template <class T> 
    struct contains_impl<null_type, T>
    {
      static const bool value = false;
    };

    // type list olmayan query'ler için
    template <class T>
    struct contains_impl<T, T>
    {
      static const bool value = true;
    };

    // liste içerisinde (herhangi bir iterasyon içerisinde) head elemanı T ile aynı ise
    template <class T, class Tail>
    struct contains_impl<type_list<T, Tail>, T>
    {
      static const bool value = true;
    };

    // liste head pozisyonundaki eleman ile T farklı ise, listeyi sırayla tarayarak arar
    template <class Head, class Tail, class T>
    struct contains_impl<type_list<Head, Tail>, T>
    {
      // search for next type in the list
      static const bool value = contains_impl<Tail, T>::value;
    };
} // namespace impl.
} // namespace tql.