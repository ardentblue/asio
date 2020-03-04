//
// traits/execute_member.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_EXECUTE_MEMBER_HPP
#define ASIO_TRAITS_EXECUTE_MEMBER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"

namespace asio {
namespace traits {

template <typename T, typename F, typename = void>
struct execute_member_default;

template <typename T, typename F, typename = void>
struct execute_member;

} // namespace traits
namespace detail {

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT)

template <typename T, typename F, typename = void>
struct execute_member_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename F>
struct execute_member_trait<T, F,
  typename void_type<
    decltype(declval<T>().execute(declval<F>()))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    declval<T>().execute(declval<F>()));

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    declval<T>().execute(declval<F>())));
};

#else // defined(ASIO_HAS_DECLTYPE)
      //   && defined(ASIO_HAS_NOEXCEPT)

template <typename T, typename F>
struct execute_member_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename F>
struct execute_member_trait<T, const F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<T, const F&> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T, F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T, const F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T, const F&> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T&, F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T&, const F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T&, const F&> :
  traits::execute_member<T, F>
{
};

#if defined(ASIO_HAS_MOVE)

template <typename T, typename F>
struct execute_member_trait<T&&, F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<T, F&&> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<T&&, const F> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T, F&&> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<T&&, const F&> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<const T&, F&&> :
  traits::execute_member<T, F>
{
};

template <typename T, typename F>
struct execute_member_trait<T&&, F&&> :
  traits::execute_member<T, F>
{
};

#endif //  defined(ASIO_HAS_MOVE)

#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename F, typename>
struct execute_member_default : detail::execute_member_trait<T, F>
{
};

template <typename T, typename F, typename>
struct execute_member : execute_member_default<T, F>
{
};

} // namespace traits
} // namespace asio

#endif // ASIO_TRAITS_EXECUTE_MEMBER_HPP
