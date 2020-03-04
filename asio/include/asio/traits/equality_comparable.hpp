//
// traits/equality_comparable.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_EQUALITY_COMPARABLE_HPP
#define ASIO_TRAITS_EQUALITY_COMPARABLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"

namespace asio {
namespace detail {

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT)

template <typename T, typename = void>
struct equality_comparable_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T>
struct equality_comparable_trait<T,
  typename void_type<
    decltype(
      static_cast<bool>(declval<const T>() == declval<const T>()),
      static_cast<bool>(declval<const T>() != declval<const T>())
    )
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept =
    noexcept(declval<const T>() == declval<const T>())
      && noexcept(declval<const T>() != declval<const T>()));
};

#else // defined(ASIO_HAS_DECLTYPE)
      //   && defined(ASIO_HAS_NOEXCEPT)

template <typename T>
struct equality_comparable_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename = void>
struct equality_comparable_default : detail::equality_comparable_trait<T>
{
};

template <typename T, typename = void>
struct equality_comparable : equality_comparable_default<T>
{
};

} // namespace traits
} // namespace asio

#endif // ASIO_TRAITS_EQUALITY_COMPARABLE_HPP
