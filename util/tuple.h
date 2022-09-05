#pragma once

#include <tuple>

namespace util {

template<size_t N, typename Tuple_Type>
struct tuple_trunc {
  typedef std::tuple<> type;
};

template<size_t N, typename Head, typename... Tail>
struct tuple_trunc<N, std::tuple<Head, Tail...>>
{
  typedef typename tuple_trunc<N-1, std::tuple<Tail...>>::type type;
};

template<typename Head, typename... Tail>
struct tuple_trunc<0, std::tuple<Head, Tail...>>
{
  typedef std::tuple<Head, Tail...> type;
};

} // namespace util

