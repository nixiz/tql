// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "include/tql.hpp"
#include "include/tuple_support.hpp"
#include <iostream>
#include <type_traits>

using namespace tql;
namespace poc
{
  typedef type_list<int, type_list<float, null_type> > AList;
  typedef type_list<double, type_list<long, null_type> > BList;
  typedef append<AList, BList>::result ResultList;
  typedef type_list<int, type_list<float, type_list<double, type_list<long, null_type> > > > ProofList;
  static_assert(std::is_same<ProofList, ResultList>::value, "append doesn't work well!");

  // typedef create_tlist<signed char, short int, int, long int>::result SignedIntegrals;
  typedef type_list<char, type_list<signed char, type_list<unsigned char, null_type> > > CharList;
  typedef type_at<CharList, 2>::result should_uchar;
  static_assert(std::is_same<should_uchar, unsigned char>::value, "type_at doesn't work well!");
  static constexpr int indexof_unsignedchar = index_of<CharList, unsigned char>::value;
  static_assert(indexof_unsignedchar == 2, "index_of doesn't work well!");

  typedef append<empty_list, char>::result single_char_list;
  typedef append<single_char_list, typename create_tlist<unsigned char, signed char>::result >::result char_list;
  typedef erase<char_list, unsigned char>::result char_list_erased;
  static_assert(std::is_same<char_list_erased, create_tlist_t<char, signed char> >::value, "erase doesn't work well!");

  typedef create_tlist_t<int, float, int, float> ints_and_floats;
  typedef erase_all<ints_and_floats, int>::result only_floats;
  static_assert(std::is_same<only_floats, create_tlist_t<float, float> >::value, "erase_all doesn't work well!");

  typedef distinct<only_floats>::result only_one_float;
  static_assert(std::is_same<only_one_float, create_tlist_t<float> >::value, "erase_all doesn't work well!");

  typedef replace_first<ints_and_floats, float, double>::result float_replaced_with_double;
  static_assert(std::is_same<float_replaced_with_double, create_tlist_t<int, double, int, float> >::value, "replace_first doesn't work well!");

  using list_of_types = create_tlist_t<int, double, float>;
  using begin_type = begin<list_of_types>::iter::value;
  static_assert(std::is_same<begin_type, int>::value, "begin<> iterator doesn't work well!");

  using end_type = end<list_of_types>::iter::value;
  static_assert(std::is_same<end_type, null_type>::value, "end<> iterator doesn't work well!");
  static_assert(end<list_of_types>::iter::pos == 3, "iterator position indicator doesn't work well!");
  
  using begin_type2 = begin<empty_list>::iter::value;
  using end_type2 = end<empty_list>::iter::value;
  static_assert(std::is_same<begin_type2, end_type2>::value, "begin or end<null_type> iterator doesn't work well!");
  
  using begin_iter = begin<list_of_types>;
  static_assert(std::is_same<begin_iter::iter::value, int>::value, "begin<> iterator doesn't work well!");
  static_assert(begin_iter::iter::pos == 0, "begin<> iterator doesn't work well!");

  using next_iter = next< list_of_types, begin_iter::iter >;
  static_assert(std::is_same<next_iter::iter::value, double>::value, "next<> iterator doesn't work well!");
  static_assert(next_iter::iter::pos == 1, "next<> iterator doesn't work well!");

  using next_iter_2 = next< list_of_types, next_iter::iter >;
  static_assert(std::is_same<next_iter_2::iter::value, float>::value, "next<> iterator doesn't work well!");
  static_assert(next_iter_2::iter::pos == 2, "next<> iterator doesn't work well!");

  using next_iter_3 = next< list_of_types, next_iter_2::iter >;
  static_assert(std::is_same<next_iter_3::iter::value, null_type>::value, "next<> iterator doesn't work well!");
  static_assert(next_iter_3::iter::pos == 3, "next<> iterator doesn't work well!");
  static_assert(std::is_same<next_iter_3::iter, end<list_of_types>::iter>::value, "next<> iterator doesn't work well!");

  using next_iter_4 = next< list_of_types, next_iter_3::iter >;
  static_assert(std::is_same<next_iter_4::iter, null_type>::value, "next<> iterator doesn't work well!");

  using next_iter_5 = next< list_of_types, next_iter_4::iter >;
  static_assert(std::is_same<next_iter_5::iter, null_type>::value, "next<> iterator doesn't work well!");

} // namespace poc

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

