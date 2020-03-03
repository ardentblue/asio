//
// traits/query_member.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_TRAITS_QUERY_MEMBER_HPP
#define ASIO_TRAITS_QUERY_MEMBER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"

namespace asio {
namespace traits {

template <typename T, typename Property, typename = void>
struct query_member_default;

template <typename T, typename Property, typename = void>
struct query_member;

} // namespace traits
namespace detail {

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT)

template <typename T, typename Property, typename = void>
struct query_member_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct query_member_trait<T, Property,
  typename void_type<
    decltype(declval<T>().query(declval<Property>()))
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    declval<T>().query(declval<Property>()));

  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    declval<T>().query(declval<Property>())));
};

#else // defined(ASIO_HAS_DECLTYPE)
      //   && defined(ASIO_HAS_NOEXCEPT)

template <typename T, typename Property>
struct query_member_trait
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct query_member_trait<T, const Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<T, const Property&> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T, Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T, const Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T, const Property&> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T&, Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T&, const Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T&, const Property&> :
  traits::query_member<T, Property>
{
};

#if defined(ASIO_HAS_MOVE)

template <typename T, typename Property>
struct query_member_trait<T&&, Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<T, Property&&> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<T&&, const Property> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T, Property&&> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<T&&, const Property&> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<const T&, Property&&> :
  traits::query_member<T, Property>
{
};

template <typename T, typename Property>
struct query_member_trait<T&&, Property&&> :
  traits::query_member<T, Property>
{
};

#endif //  defined(ASIO_HAS_MOVE)

#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename>
struct query_member_default :
  detail::query_member_trait<T, Property>
{
};

template <typename T, typename Property, typename>
struct query_member :
  query_member_default<T, Property>
{
};

} // namespace traits
} // namespace asio

#endif // ASIO_TRAITS_QUERY_MEMBER_HPP
