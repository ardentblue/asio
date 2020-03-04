//
// traits/execute_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_EXECUTE_FREE_HPP
#define ASIO_TRAITS_EXECUTE_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"

namespace asio {
namespace traits {

template <typename T, typename F, typename = void>
struct execute_free_default;

template <typename T, typename F, typename = void>
struct execute_free;

} // namespace traits
namespace detail {

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT)

#if defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename F, typename = void>
struct execute_free_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename F>
struct execute_free_trait<T, F,
  typename void_type<
    decltype(execute(declval<T>(), declval<F>()))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    execute(declval<T>(), declval<F>()));

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    execute(declval<T>(), declval<F>())));
};

#else // defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename F,
  typename = decltype(execute(declval<T>(), declval<F>()))>
struct execute_free_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    execute(declval<T>(), declval<F>()));

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    execute(declval<T>(), declval<F>())));
};

struct no_execute {};

template <typename T, typename F>
no_execute execute(T, F);

template <typename T, typename F>
struct execute_free_trait<T, F, no_execute>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(ASIO_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(ASIO_HAS_DECLTYPE)
      //   && defined(ASIO_HAS_NOEXCEPT)

template <typename T, typename F>
struct execute_free_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename F>
struct execute_free_trait<T, const F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<T, const F&> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T, F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T, const F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T, const F&> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T&, F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T&, const F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T&, const F&> :
  traits::execute_free<T, F>
{
};

#if defined(ASIO_HAS_MOVE)

template <typename T, typename F>
struct execute_free_trait<T&&, F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<T, F&&> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<T&&, const F> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T, F&&> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<T&&, const F&> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<const T&, F&&> :
  traits::execute_free<T, F>
{
};

template <typename T, typename F>
struct execute_free_trait<T&&, F&&> :
  traits::execute_free<T, F>
{
};

#endif //  defined(ASIO_HAS_MOVE)

#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename F, typename>
struct execute_free_default : detail::execute_free_trait<T, F>
{
};

template <typename T, typename F, typename>
struct execute_free : execute_free_default<T, F>
{
};

} // namespace traits
} // namespace asio

#endif // ASIO_TRAITS_EXECUTE_FREE_HPP
