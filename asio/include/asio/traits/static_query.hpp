//
// traits/static_query.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_STATIC_QUERY_HPP
#define ASIO_TRAITS_STATIC_QUERY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"

namespace asio {
namespace detail {

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property, typename = void>
struct static_query_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
};

template <typename T, typename Property>
struct static_query_trait<T, Property,
  typename void_type<
    decltype(Property::template static_query_v<T>)
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(Property::template static_query_v<T>);

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept =
    noexcept(Property::template static_query_v<T>));

  static ASIO_CONSTEXPR result_type value() noexcept(is_noexcept)
  {
    return Property::template static_query_v<T>;
  }
};

#else // defined(ASIO_HAS_DECLTYPE)
      //   && defined(ASIO_HAS_NOEXCEPT)
      //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct static_query_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct static_query_default : detail::static_query_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct static_query : static_query_default<T, Property>
{
};

} // namespace traits
} // namespace asio

#endif // ASIO_TRAITS_STATIC_QUERY_HPP
