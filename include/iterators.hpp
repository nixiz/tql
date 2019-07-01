#pragma once

#include "src/config.hpp"
#include "include/type_list.hpp"
#include "include/length.hpp"
#include "include/index_of.hpp"
#include "include/type_at.hpp"

namespace tql
{
  // usage: begin<tlist>::value -> can be fist type of list or null_type (aka: end)
  template <class Tlist> struct begin;
  template <class Tlist> struct end;
  template <class Tlist, class Iter = typename begin<Tlist>::iter> struct next;

namespace impl
{
  // usage: begin<tlist>::value -> can be fist type of list or null_type (aka: end)
  //template <class Tlist> struct begin;
  template <typename T, unsigned int index = 0>
  struct iterator
  {
    static const int pos = index;
    typedef T value;
  };

} // namespace impl.

  template <class Tlist>
  struct end
  {
  private:
    // storing last element position for distance and iteration calculation
    static const int tmp_pos = length<Tlist>::value;
    // pos indikatörü her zaman positif veya 0'a eşit olmak zorunda
    // aksi takdirde distance hesaplamalarında sıkıntılar yaşayabiliriz!
    static const int pos = tmp_pos > 0 ? tmp_pos : 0;
  public:
    typedef impl::iterator<null_type, pos> iter;
  };

  /// tql::create_tlist_t<X,Y,Z> list;
  /// begin<list>::value -> X
  template <typename Head, typename Tail>
  struct begin<type_list<Head, Tail>>
  {
    typedef impl::iterator<Head> iter;
  };

  template <>
  struct begin<null_type>
  {
    typedef impl::iterator<null_type> iter;
  };

  template <>
  struct begin<empty_list>
  {
    typedef impl::iterator<null_type> iter;
  };

  template <class Tlist, class Iterator >
  struct next
  {
  private:
    static constexpr int prev_pos = index_of<Tlist, typename Iterator::value>::value;
    // static constexpr int next_pos = (prev_pos + 1 == length<Tlist>::value) ? prev_pos : prev_pos + 1; 
    static constexpr int next_pos = prev_pos + 1; 
  public:
    typedef impl::iterator<typename type_at<Tlist, next_pos >::result, next_pos > iter;
  };

  // break the iteration if next iter is end
  template <class Tlist>
  struct next<Tlist, typename end<Tlist>::iter >
  {
    typedef null_type iter;
  };

  template <class Tlist>
  struct next<Tlist, null_type >
  {
    typedef null_type iter;
  };
  
} // namespace tql.