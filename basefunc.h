/*
 * basefunc.h - 
 */

#ifndef BASEFUNC_INC_SEEN
#define BASEFUNC_INC_SEEN

// basefunc
namespace detail
{
  template<int N> struct array_size_t { char result[N]; };
  template<class T,int N> array_size_t<N> array_size(T (&x)[N]);
}

#define lengthof(arr) (sizeof(detail::array_size(arr).result))
// #define lengthof(arr) (sizeof(arr)/sizeof((arr)[0]))

#endif // BASEFUNC_INC_SEEN
