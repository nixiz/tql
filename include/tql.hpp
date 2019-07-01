#pragma once
#include "src/config.hpp"

// template query language
namespace tql
{

  template <class T, class U = null_type>
  struct type_list
  {
    typedef T Head;
    typedef U Tail;
  };

  template <class... Elems> struct create_tlist;
  template <class ...Elems>
  using create_tlist_t = typename create_tlist<Elems...>::result;
  // empty list 
  using empty_list = type_list<null_type, null_type>;

  // access and capasity operations
  template <class Tlist> struct length;
  template <class Tlist, unsigned int index> struct type_at;
  template <class TList, class T> struct index_of;
  template <class TList, class T> struct contains;

  // modifiers
  template <class Tlist, class T> struct append;
  template <class Tlist, class T> struct erase;
  template <class Tlist, class T> struct erase_all;

  // operations
  template <class Tlist, class T, class U> struct replace;
  template <class Tlist> struct distinct;

  // iterators
  // usage: begin<tlist>::value -> can be fist type of list or null_type (aka: end)
  template <class Tlist> struct begin;
  template <class Tlist> struct end;
  template <class Tlist, class Iter = typename begin<Tlist>::iter> struct next;

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

	// ---------------------------------------------------------------------  //

  // ---------------------------------------------------------------------  //
  //  template <class Tlist> struct length; //

  namespace impl
  {
    template <class Tlist> struct length_impl;
    
    template <> struct length_impl<null_type>
    {
      static const int value = 0;
    };

    template <> struct length_impl<empty_list>
    {
      static const int value = 0;
    };

    template <class T, class U>
    struct length_impl<type_list<T, U> >
    {
      static const int value = 1 + length_impl<U>::value;
    };
  } // namespace impl

  template <class Tlist> struct length
  {
    static constexpr int value = impl::length_impl<Tlist>::value; 
    typedef decltype(value) value_type;
  };


	// ---------------------------------------------------------------------  //
	// template <class Tlist, unsigned int index> struct type_at; //

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
  } // namespace impl

  template <class Tlist, unsigned int index> 
  struct type_at
  {
    typedef typename impl::type_at_impl<Tlist, index>::result result; 
    typedef result value_type;
  };


	// ---------------------------------------------------------------------  //
	// template <class TList, class T> struct index_of;

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
  } // namespace impl

  template <class TList, class T> 
  struct index_of
  {
    static constexpr int value = impl::index_of_impl<TList, T>::value;
    typedef decltype(value) value_type;
  };


	// ---------------------------------------------------------------------  //

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

  } // namespace impl

  template <class TList, class T> 
  struct contains
  {
    static constexpr bool value = impl::contains_impl<TList, T>::value;
    typedef decltype(value) value_type;
  };

  // ---------------------------------------------------------------------  //

  namespace impl
  {
    template <class Tlist, class T> struct append_impl;
    // Tlist'e T tipini ekler
    // template <class Tlist, class T> struct append;
    // özelleştirme : null tipler için
    template <>
    struct append_impl<null_type, null_type>
    {
      typedef null_type result;
    };
    // özelleştirme (1)
    template <class T>
    struct append_impl<null_type, T>
    {
      typedef type_list<T, null_type> result;
    };

    // özelleştirme (2)
    template <class T>
    struct append_impl<type_list<null_type, null_type>, T>
    {
      typedef type_list<T, null_type> result;
    };

    // özelleştirme (3)
    template <class Head, class Tail>
    struct append_impl<null_type, type_list<Head, Tail> >
    {
      typedef type_list<Head, Tail> result;
    };

    // özelleştirme (4)
    template <class Head, class Tail, class T>
    struct append_impl<type_list<Head, Tail>, T>
    {
      typedef type_list<Head, typename append_impl<Tail, T>::result> result;
    };
  } // namespace impl

	template <class Tlist, class T>
  struct append
  {
    typedef typename impl::append_impl<Tlist, T>::result result;
    typedef result value_type;
  };


	// ---------------------------------------------------------------------  //

  namespace impl
  {
    template <class Tlist, class T> struct erase_impl;

    template <class T> 
    struct erase_impl<null_type, T>
    {
      typedef null_type result;
    };

    template <class T, class Tail>
    struct erase_impl<type_list<T, Tail>, T>	// eðer T tipi typelist içerisindeki Head ise o zaman sonuç head dýþýnda kalan Tail'dir
    {
      typedef Tail result;
    };

    template <class Head, class Tail, class T>
    struct erase_impl<type_list<Head, Tail>, T>	// T tipi type_list içerisinde yoksa, list üzerinde itere ederek aramaya devam eder
    {
      typedef type_list<Head, typename erase_impl<Tail, T>::result> result;
    };
  }

	// T tipini Typelistten siler (ilk bulduðu yerden siler)
	template <class Tlist, class T> 
  struct erase
  {
    typedef typename impl::erase_impl<Tlist, T>::result result;
    typedef result value_type;
  };

	// ---------------------------------------------------------------------  //

  namespace impl
  {
    template <class Tlist, class T> struct erase_all_impl;

    template <class T>
    struct erase_all_impl<null_type, T>
    {
      typedef null_type result;
    };

    template <class T, class Tail>
    struct erase_all_impl<type_list<T, Tail>, T>
    {
      // get all the way down the list removing the type
      typedef typename erase_all_impl<Tail, T>::result result;
    };

    template <class Head, class Tail, class T>
    struct erase_all_impl<type_list<Head, Tail>, T>
    {
      // go all the way down the list removing the type
      typedef type_list<Head, typename erase_all_impl<Tail, T>::result> result;
    };
  }
	// T tipini Typelistten siler (bütün liste içerisinde arar ve siler)
	template <class Tlist, class T> 
  struct erase_all
  {
    typedef typename impl::erase_all_impl<Tlist, T>::result result;
    typedef result value_type;
  };


  // ---------------------------------------------------------------------  //

  namespace impl
  {
  	template <class Tlist> struct distinct_impl;

    template <> 
    struct distinct_impl<null_type>
    {
      typedef null_type result;
    };

    template <>
    struct distinct_impl<empty_list>
    {
      typedef null_type result;
    };

    /*
    * If TList is null_type, then result is null_type.
    * Else
    *   Apply NoDuplicates to TList::Tail, obtaining a temporary typelist L1.
    *   Apply erase to L1 and TList::Head. Obtain L2 as the result.
    *   result is a typelist whose head is TList::Head and whose tail is L2.
    */
    template <class Head, class Tail>
    struct distinct_impl<type_list<Head, Tail>>
    {
    private:
      typedef typename distinct_impl<Tail>::result L1;
      typedef typename erase<L1, Head>::result L2;
    public:
      typedef type_list<Head, L2> result;
    };

  } // namespace impl

	// Removes duplicates from the list
	template <class Tlist> 
  struct distinct
  {
    typedef typename impl::distinct_impl<Tlist>::result result;
    typedef result value_type;
  };


	/*
	 * Why was erase enough when erase_all would have seemed appropriate? We want to remove all
	 * duplicates for a type, right? The answer is that erase is applied after the recursion to NoDuplicates.
	 * This means we erase a type from a list that already has no duplicates, so at most one instance of the type to
	 * be erased will appear. This recursive programming is quite interesting.
	 */
	 // ---------------------------------------------------------------------  //

	/*
	 * If TList is null_type, then result is null_type.
	 * Else
	 * If the head of the typelist TList is T, then result is a typelist with U as its head and
	 *   TList::Tail as its tail.
	 *   Else result is a typelist with TList::Head as its head and the result of applying 
	 *   replace to TList, T, and U as its tail.
	 */
	
  namespace impl
  {
  	template <class Tlist, class T, class U> struct replace_impl;

    template <class T, class U> 
    struct replace_impl<null_type, T, U>
    {
      typedef null_type result;
    };

    template <class T, class Tail, class U>
    struct replace_impl<type_list<T, Tail>, T, U>
    {
      typedef type_list<U, Tail> result;
    };

    template <class Head, class Tail, class T, class U>
    struct replace_impl<type_list<Head, Tail>, T, U>
    {
      typedef type_list<Head, typename replace_impl<Tail, T, U>::result> result;
    };
  } // namespace impl

  // Removes duplicates from the list
	template <class Tlist, class T, class U> 
  struct replace
  {
    typedef typename impl::replace_impl<Tlist, T, U>::result result;
    typedef result value_type;
  };


  // ---------------------------------------------------------------------  //

  /* Create list helpers for c++11 and higher standards*/
  namespace impl
  {
    // generic tpyelist creator decleration
    template <typename Tlist, typename ...Elems>
    struct create_tlist_helper;

    template <typename Tlist>
    struct create_tlist_helper<Tlist>
    {
      using result = Tlist;
    };

    template <typename Tlist, typename Elem>
    struct create_tlist_helper<Tlist, Elem>
    {
      using result = typename tql::append<Tlist, Elem>::result;
    };

    template <typename Tlist, typename Elem, typename ...Others>
    struct create_tlist_helper<Tlist, Elem, Others...>
    {
      typedef typename tql::append<Tlist, Elem>::result tlist;
      using result = typename create_tlist_helper<tlist, Others...>::result;
    };
  } // namespace impl.

  template <class ...Elems>
  struct create_tlist
  {
    using result = typename impl::create_tlist_helper<tql::empty_list, Elems...>::result;
  };

  // ---------------------------------------------------------------------  //

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
  } // namespace detail.

  template <class Tlist>
  struct end
  {
  private:
    // storing last element position for distance and iteration calculation
    static const int tmp_pos = length<Tlist>::value;
    // pos indikatörü her zaman positif veya 0'a eşit olmak zorunda
    // aksi takdirde distance hesaplamalarında sıkıntılar yaşayabiliriz!
    static const int pos = tmp_pos > 0 ? tmp_pos - 1 : 0;
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
    static const int prev_pos = index_of<Tlist, typename Iterator::value>::value;
  public:
    typedef impl::iterator<typename type_at<Tlist, prev_pos + 1 >::result> iter;
  };

  template <class Tlist>
  struct next<Tlist, impl::iterator<null_type> >
  {
    typedef null_type iter;
  };

  template <class Tlist>
  struct next<Tlist, null_type >
  {
    typedef null_type iter;
  };

  // ------------------------- query support ------------------------- //

  namespace impl
  {
    template <class tag_dispatch, class Tlist, class _From, class Iter,
      template <class, class> class predicate>
    struct query_p;
    // template tag dispatch helpers
    struct use_from_predicate {};
    struct use_to_predicate {};
  } // namespace detail.

  template <class T1 = null_type, class T2 = null_type>
  struct place_holder {};

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
    static const bool value = impl::query_p<impl::use_from_predicate, Tlist, _From, begin_iter, predicate>::value;
  };

  template <
    template <class, class> class predicate,
    class Tlist, class _To>
  struct query<predicate, Tlist, place_holder<null_type, _To>>
  {
  private:
    using begin_iter = typename begin<Tlist>::iter;
  public:
    static const bool value = impl::query_p<impl::use_to_predicate, Tlist, _To, begin_iter, predicate>::value;
  };

  namespace impl
  {

    template <class Tlist, class _To, class Iter,
      template <class, class> class predicate>
    struct query_p<use_to_predicate, Tlist, _To, Iter, predicate>
    {
    public:
      static const bool value = predicate<typename Iter::value, _To>::value == true ? true : query_p<use_to_predicate, Tlist, _To, typename next<Tlist, Iter>::iter, predicate>::value;
    };

    template <class Tlist, class _From, class Iter,
      template <class, class> class predicate>
    struct query_p<use_from_predicate, Tlist, _From, Iter, predicate>
    {
    public:
      static const bool value = predicate<_From, typename Iter::value>::value == true ? true : query_p<use_from_predicate, Tlist, _From, typename next<Tlist, Iter>::iter, predicate>::value;
    };


    template <class Tlist, class T, template <class, class> class predicate>
    struct query_p<use_to_predicate, Tlist, T, null_type, predicate>
    {
    public:
      static const bool value = false;
    };

    template <class Tlist, class _From, unsigned int index, template <class, class> class predicate>
    struct query_p<use_from_predicate, Tlist, _From, impl::iterator<null_type, index>, predicate>
    {
    public:
      static const bool value = false;
    };

    template <class Tlist, class _From, unsigned int index, template <class, class> class predicate>
    struct query_p<use_to_predicate, Tlist, _From, impl::iterator<null_type, index>, predicate>
    {
    public:
      static const bool value = false;
    };

  } // namespace detail.

  // ---------------------------------------------------------------------  //

} // namespace tl {typelist}
