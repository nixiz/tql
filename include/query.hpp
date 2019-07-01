#pragma once
#include "src/config.hpp"
#include "src/query_impl.hpp"

namespace tql
{
  
  template <class T1 = null_type, class T2 = null_type>
  struct place_holder {};

  // query with std type traits support. usage: quert<tlist, T, std::is_convertible>::value
  // T tipi liste içindeki tiplerden birine dönüştürülebilirse value: 'true' olacaktır.
  //template <
  //  class Tlist, class Pair,
  //  template <class, class> class predicate >
  //struct query;
  template <
    template <class, class> class predicate,
    class Tlist, class Pair>
  struct query;

  template <
    template <class, class> class predicate,
    class Tlist, class _T>
    struct query
  {
  public:
    static typename predicate<Tlist, _T>::value_type value = predicate<Tlist, _T>::value;
    //static const bool value = predicate<Tlist, _T>::value;
  };

  template <
    template <class, class> class predicate,
    class Tlist, class _From>
  struct query<predicate, Tlist, place_holder<_From, null_type>>
  {
  private:
    using begin_iter = typename begin<Tlist>::iter;
  public:
    static const bool value = impl::query_impl<impl::use_from_predicate, Tlist, _From, begin_iter, predicate>::value;
  };

  template <
    template <class, class> class predicate,
    class Tlist, class _To>
  struct query<predicate, Tlist, place_holder<null_type, _To>>
  {
  private:
    using begin_iter = typename begin<Tlist>::iter;
  public:
    static const bool value = impl::query_impl<impl::use_to_predicate, Tlist, _To, begin_iter, predicate>::value;
  }; 

}