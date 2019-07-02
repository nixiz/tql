# TQL Features
<!-- TQL ile derleyici zamanında çalışacak tip listeleri yaratabilir, listeye eleman ekleyebilir, çıkarabilir ve bir çok işlemi gerçekleştirebilirsiniz. --> 
<!-- TQL query desteği sayesinde gelişmiş SFINAE yazabilir veya derleme zamanında tuple işlemlerini gerçekleştirebilirsiniz. -->

* Create and Modify Type Lists
  * Append
  * Erase
  * Replace
  * Distinct
* Access and Capasity Operations
  * Length
  * TypeAt
  * IndexOf
  * Contains
* Iterator Support 
  * Begin
  * End
  * Next
* Query Support
* STL Type Traits Library and SFIANE classes
* STL Tuple Support

# Usage
```cpp
constexpr auto get_tuple()
{
  return std::make_tuple(3, 0.8f, 3.14);  
}

template <typename tuple, 
  typename = std::enable_if_t<tql::query<tql::contains, tuple, int>::value> >
constexpr int get_int_from_tuple(tuple&& t) // enables if tuple have int type
{
  return std::get<tql::query<tql::index_of, tuple, int>::value>(std::forward<tuple>(t));
}

template <typename tuple, 
  typename = std::enable_if_t<tql::query<tql::contains, tuple, float>::value> >
constexpr float get_float_from_tuple(tuple&& t) // enables if tuple have float type
{
  return std::get<tql::query<tql::index_of, tuple, float>::value>(std::forward<tuple>(t));
}

...
// all operations will be done at compile time.

constexpr int res = poc_tuple::get_int_from_tuple(poc_tuple::get_tuple());
static_assert(res == 3, "");

constexpr float res_f = poc_tuple::get_float_from_tuple(poc_tuple::get_tuple());
static_assert(res_f == 0.8f, "");
```

## SFINAE Support

Create tuple or type_list with types:

```cpp
struct abstract_base {
  virtual void abstract_foo() = 0;
};

struct base {
  virtual void virtual_foo() {}
};

struct derived_abstract : public abstract_base {
  void abstract_foo() override {}
};

struct derived_virtual_not_override : public base { };

struct derived_virtual_override : public base {
  void virtual_foo() override {}
};

struct derived_both : public base, public abstract_base {
  void virtual_foo() override {}
  void abstract_foo() override {}
};

using test_tuple_integers = std::tuple<unsigned, int, short>;
using test_tuple_classes = std::tuple<derived_abstract, derived_both>;

```

TQL query can be used with STL type traits library as well

```cpp
// stl type_traits and tuples can be used with tql::query. 
static_assert(tql::query<std::is_base_of, test_tuple_classes, tql::place_holder<abstract_base, null_type>>::value, "");
static_assert(tql::query<std::is_base_of, tql::create_tlist_t<derived_abstract, derived_both>, tql::place_holder<abstract_base>>::value, "");

static_assert(tql::query<tql::contains, test_tuple_integers, short>::value, "");
static_assert(!tql::query<tql::contains, test_tuple_integers, char>::value, "");
```

## Iterator Support For Type Lists and Tuples

```cpp
using list_of_types = create_tlist_t<int, double, float>;
using begin_type = begin<list_of_types>::iter::value;
static_assert(std::is_same<begin_type, int>::value, "begin<> iterator doesn't work!");

using end_type = end<list_of_types>::iter::value;
static_assert(std::is_same<end_type, null_type>::value, "end<> iterator doesn't work!");
static_assert(end<list_of_types>::iter::pos == 3, "iterator position indicator doesn't work!");

using begin_type2 = begin<empty_list>::iter::value;
using end_type2 = end<empty_list>::iter::value;
static_assert(std::is_same<begin_type2, end_type2>::value, "begin or end<null_type> iterator doesn't work!");

using begin_iter = begin<list_of_types>;
static_assert(std::is_same<begin_iter::iter::value, int>::value, "begin<> iterator doesn't work!");
static_assert(begin_iter::iter::pos == 0, "begin<> iterator doesn't work!");

using next_iter = next< list_of_types, begin_iter::iter >;
static_assert(std::is_same<next_iter::iter::value, double>::value, "next<> iterator doesn't work!");
static_assert(next_iter::iter::pos == 1, "next<> iterator doesn't work!");

using next_iter_2 = next< list_of_types, next_iter::iter >;
static_assert(std::is_same<next_iter_2::iter::value, float>::value, "next<> iterator doesn't work!");
static_assert(next_iter_2::iter::pos == 2, "next<> iterator doesn't work!");

using next_iter_3 = next< list_of_types, next_iter_2::iter >;
static_assert(std::is_same<next_iter_3::iter::value, null_type>::value, "next<> iterator doesn't work!");
static_assert(next_iter_3::iter::pos == 3, "next<> iterator doesn't work well!");
static_assert(std::is_same<next_iter_3::iter, end<list_of_types>::iter>::value, "next<> iterator doesn't work!");

using next_iter_4 = next< list_of_types, next_iter_3::iter >;
static_assert(std::is_same<next_iter_4::iter, null_type>::value, "next<> iterator doesn't work!");

using next_iter_5 = next< list_of_types, next_iter_4::iter >;
static_assert(std::is_same<next_iter_5::iter, null_type>::value, "next<> iterator doesn't work!");
```


## License

This library is licensed under the MIT License - see the LICENSE file for details.
