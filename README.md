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
#include "include/tql.hpp"
// for tuple support include tuple_support.hpp
#include "include/tuple_support.hpp"

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

int main()
{
  // all operations will be done at compile time.
  constexpr int res = get_int_from_tuple(poc_tuple::get_tuple());
  static_assert(res == 3, "");

  static_assert(tuple_have_type<std::tuple<int, float, std::string>, std::string>::result, "");
  return 0;
}
```

## SFINAE Usage

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

// create tuple classes:
using test_tuple_classes = std::tuple<derived_abstract, derived_both>;

// checking at compile time that all types are the base type of 'abstract_base' class test_tuple_classes have    
static_assert(tql::query<std::is_base_of, test_tuple_classes, tql::place_holder<abstract_base, null_type>>::value, "");

using test_tuple_integers = std::tuple<unsigned, int, short>;

static_assert(tql::query<tql::contains, test_tuple_integers, short>::value, "");
static_assert(!tql::query<tql::contains, test_tuple_integers, char>::value, "");
```

## Iterator Support For Type Lists and Tuples

```cpp
// create type_list with types:
using list_of_types = create_tlist_t<int, double, float>;

// start the iteration
// result of iterator operations have current type and position of iteration  
using begin_iter = begin<list_of_types>;

// checking the result of iteration is correct 
static_assert(begin_iter::iter::pos == 0, "");
static_assert(std::is_same<begin_iter::iter::value, int>::value, "");

using next_iter = next< list_of_types, begin_iter::iter >;
static_assert(next_iter::iter::pos == 1, "");
static_assert(std::is_same<next_iter::iter::value, double>::value, "");

using next_iter_2 = next< list_of_types, next_iter::iter >;
static_assert(next_iter_2::iter::pos == 2, "");
static_assert(std::is_same<next_iter_2::iter::value, float>::value, "");

using next_iter_3 = next< list_of_types, next_iter_2::iter >;
static_assert(next_iter_3::iter::pos == 3, "");
static_assert(std::is_same<next_iter_3::iter::value, null_type>::value, "");
static_assert(std::is_same<next_iter_3::iter, end<list_of_types>::iter>::value, "");

// overflowed iterations will return always null_type as a result
using next_iter_4 = next< list_of_types, next_iter_3::iter >;
static_assert(std::is_same<next_iter_4::iter, null_type>::value, "");

using next_iter_5 = next< list_of_types, next_iter_4::iter >;
static_assert(std::is_same<next_iter_5::iter, null_type>::value, "");
```

## License

This library is licensed under the MIT License - see the LICENSE file for details.
