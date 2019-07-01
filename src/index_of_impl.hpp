#pragma once
#include "include/type_list.hpp"

namespace tql
{
namespace impl
{
    template <class TList, class T> struct index_of_impl;

    template <class T>
    struct index_of_impl<null_type, T>
    {
      static constexpr int value = -1; // invalid num
    };

    template <class T, class Tail>
    struct index_of_impl<type_list<T, Tail>, T>
    {
      // aranan tip typelist içerisindeki head 
      // indeksindeyse, arama tamamlanmış demektir.
      // recursion burada kırılır.
      static constexpr int value = 0;
    };

    template <class Head, class Tail, class T>
    struct index_of_impl<type_list<Head, Tail>, T>
    {
    private:
      static constexpr int temp = index_of_impl<Tail, T>::value;
    public:
      static constexpr int value = temp == -1 ? -1 : 1 + temp;
    };

} // namespace impl.
} // namespace tql.