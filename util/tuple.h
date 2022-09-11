#pragma once

#include <tuple>

namespace util {

template<size_t N, typename Tuple_Type>
struct remove_first_tuple_elements {
  typedef std::tuple<> type;
};

template<size_t N, typename Head, typename... Tail>
struct remove_first_tuple_elements<N, std::tuple<Head, Tail...>>
{
  typedef typename remove_first_tuple_elements<N - 1, std::tuple<Tail...>>::type type;
};

template<typename Head, typename... Tail>
struct remove_first_tuple_elements<0, std::tuple<Head, Tail...>>
{
  typedef std::tuple<Head, Tail...> type;
};

} // namespace util

