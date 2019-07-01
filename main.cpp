// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "include/tql.hpp"
#include <iostream>
#include <type_traits>
#include <tuple>

using namespace tql;
namespace poc
{
  typedef type_list<int, type_list<float, null_type> > AList;
  typedef type_list<double, type_list<long, null_type> > BList;
  typedef append<AList, BList>::result ResultList;
  typedef type_list<int, type_list<float, type_list<double, type_list<long, null_type> > > > ProofList;
  static_assert(std::is_same<ProofList, ResultList>::value, "Append doesn't work well!");

  typedef create_tlist<signed char, short int, int, long int>::result SignedIntegrals;
  typedef type_list<char, type_list<signed char, type_list<unsigned char, null_type> > > CharList;
  typedef type_at<CharList, 2>::result RRR;
  static const int indexof_unsignedchar = index_of<CharList, unsigned char>::value;

  typedef append<empty_list, char>::result CharlistBegin;
  typedef append<CharlistBegin, typename create_tlist<unsigned char, signed char>::result >::result CharlistEnd;
  typedef erase<CharlistEnd, unsigned char>::result CharListMid;
  typedef create_tlist<int, float, int, float>::result BunchOfIntsAndFloats;
  typedef erase_all<BunchOfIntsAndFloats, int>::result OnlyFloats;
  typedef distinct<OnlyFloats>::result OnlyOneFloat;
  typedef replace<BunchOfIntsAndFloats, float, double>::result BunchOfIntsAndDoubles;

  using list0 = create_tlist_t<int, double, float>;
  using begin_type = begin<list0>::iter::value;
  using end_type = end<list0>::iter::value;
  using end_type2 = end<empty_list>::iter::value;
  static const int p = end<list0>::iter::pos;

  using next_iter = next< list0 >;
  using next_iter_2 = next< list0, next_iter::iter >;
  using next_iter_3 = next< list0, next_iter_2::iter >;
  using next_iter_4 = next< list0, next_iter_3::iter >;
  using next_iter_5 = next< list0, next_iter_4::iter >::iter;
} // namespace poc

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



namespace tquery
{
  struct abstract_base {
    virtual void abstract_foo() = 0;
  };

  struct base {
    virtual void virtual_foo() {}
  };

  struct derived_abstract : public abstract_base
  {
    void abstract_foo() override {}
  };

  struct derived_virtual_not_override : public base
  {
    //void virtual_foo() override {}
  };

  struct derived_virtual_override : public base
  {
    void virtual_foo() override {}
  };

  struct derived_both : public base, public abstract_base
  {
    void virtual_foo() override {}
    void abstract_foo() override {}
  };


  using test_tuple_integers = std::tuple<unsigned, int, short>;
  using test_tuple_abstract_classes = std::tuple<derived_abstract, derived_both>;
  
  static_assert(tql::query<std::is_base_of, test_tuple_abstract_classes, tql::place_holder<abstract_base, null_type>>::value, "hope it will work!");
  static_assert(tql::query<std::is_base_of, tql::create_tlist_t<derived_abstract, derived_both>, tql::place_holder<abstract_base, null_type>>::value, "hope it will work!");
  static_assert(tql::query<tql::contains, test_tuple_integers, short>::value, "has_type is not working!");
  static_assert(!tql::query<tql::contains, test_tuple_integers, char>::value, "has_type is not working!");

  using test_tlist_pod = tql::create_tlist_t<double, float, int>;
  static_assert(!tql::query<tql::contains, test_tlist_pod, char>::value, "has_type is not working!");
  static_assert(tql::query<tql::contains, test_tlist_pod, int>::value, "has_type is not working!");
  static_assert(tql::query<tql::contains, int, int>::value, "has_type is not working!");
  static_assert(!tql::query<tql::contains, float, int>::value, "has_type is not working!");

  static_assert(tql::query<tql::index_of, test_tlist_pod, int>::value == 2, "has_type is not working!");

  //static_assert(!std::conjunction<std::is_base_of<abstract_base, test_tuple_abstract_classes>>::value, "qweweqweqweq");
  //static_assert(!std::is_base_of<abstract_base, test_tuple_abstract_classes>::value, "asdasdadsad");

}


int main()
{
  return 0;
}

