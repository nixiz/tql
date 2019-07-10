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
    template <class Tlist, unsigned int index> struct type_at_impl;
    template <class Head, class Tail>
    struct type_at_impl<type_list<Head, Tail>, 0>
    {
      // typelis içerisinde bulunmak istenen index kadar
      // ilerlendiðinde list içerisindeki 'Head' tipi aranan tip olacaktýr.
      typedef Head result;
    };

    template <class Head, class Tail, unsigned int index>
    struct type_at_impl<type_list<Head, Tail>, index>
    {
      // burada tail => type_list<T, U> objesi olduðu için
      // type_at<type_list<Head, Tail (type_list<T,U>)>, i> bir sonraki recursion'a
      // type_at<type_list<T,U>, i - 1> gönderiyor olacak.
      typedef typename type_at_impl<Tail, index - 1>::result result;
    };

    // typelist elemanlarý dýþarýsýnda kalan index için 
    // özelleþtirilmiþ struct bu ama içerisinde kontrollü
    // bir þekilde fail edemiyorum!..
    template <unsigned int index>
    struct type_at_impl<null_type, index>
    {
      // bu þekilde yaptýðýmda compile hatasýný supress ediyorum
      // static assert burada kullanýlamýyor!
      //struct ErrType : public null_type {};
      //typedef ErrType result; 
      typedef null_type result;
    };

} // namespace impl.
} // namespace tql.