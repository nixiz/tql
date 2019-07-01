#pragma once
#include "src/config.hpp"
#include "query.hpp"
#include <tuple>

namespace tql
{
  
  template <
    template <class, class> class predicate,
    class _From,
    class ...Ts>
  struct query<predicate, std::tuple<Ts...>, place_holder<_From, null_type>>
  {
  private:
    using tuple_list = create_tlist_t<Ts...>;
    using begin_iter = typename begin<tuple_list>::iter;
  public:
    static const bool value = impl::query_impl<impl::use_from_predicate, tuple_list, _From, begin_iter, predicate>::value;
  };

  template <
    template <class, class> class predicate,
    class _To,
    class ...Ts>
    struct query<predicate, std::tuple<Ts...>, place_holder<null_type, _To>>
  {
  private:
    using tuple_list = create_tlist_t<Ts...>;
    using begin_iter = typename begin<tuple_list>::iter;
  public:
    static const bool value = impl::query_impl<impl::use_to_predicate, tuple_list, _To, begin_iter, predicate>::value;
  };

  template <
    template <class, class> class predicate,
    class _T, class ...Ts>
    struct query<predicate, std::tuple<Ts...>, _T>
  {
  private:
    using tuple_list = create_tlist_t<Ts...>;
  public:
    static const bool value = predicate<tuple_list, _T>::value;
  };

}