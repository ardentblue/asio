//
// execution/outstanding_work.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXECUTION_OUTSTANDING_WORK_HPP
#define ASIO_EXECUTION_OUTSTANDING_WORK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/execution/executor.hpp"
#include "asio/is_applicable_property.hpp"
#include "asio/query.hpp"
#include "asio/traits/prefer_free.hpp"
#include "asio/traits/query_free.hpp"
#include "asio/traits/query_member.hpp"
#include "asio/traits/query_static_constexpr_member.hpp"
#include "asio/traits/require_free.hpp"
#include "asio/traits/require_member.hpp"
#include "asio/traits/static_query.hpp"
#include "asio/traits/static_require.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace execution {
namespace detail {
namespace outstanding_work {

template <int I> struct untracked_t;
template <int I> struct tracked_t;

} // namespace outstanding_work

template <int I = 0>
struct outstanding_work_t
{
#if defined(ASIO_HAS_VARIABLE_TEMPLATES)
  template <typename T>
  ASIO_STATIC_CONSTEXPR(bool,
    is_applicable_property_v = is_executor<T>::value);
#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

  ASIO_STATIC_CONSTEXPR(bool, is_requirable = false);
  ASIO_STATIC_CONSTEXPR(bool, is_preferable = false);
  typedef outstanding_work_t polymorphic_query_result_type;

  typedef detail::outstanding_work::untracked_t<I> untracked_t;
  typedef detail::outstanding_work::tracked_t<I> tracked_t;

  ASIO_CONSTEXPR outstanding_work_t()
    : value_(-1)
  {
  }

  ASIO_CONSTEXPR outstanding_work_t(untracked_t)
    : value_(0)
  {
  }

  ASIO_CONSTEXPR outstanding_work_t(tracked_t)
    : value_(1)
  {
  }

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_VARIABLE_TEMPLATES)
  template <typename T>
  static ASIO_CONSTEXPR
  typename traits::query_static_constexpr_member<
      T, outstanding_work_t>::result_type
  static_query()
    ASIO_NOEXCEPT_IF((
      traits::query_static_constexpr_member<
        T, outstanding_work_t
      >::is_noexcept))
  {
    return traits::query_static_constexpr_member<
        T, outstanding_work_t>::value();
  }

  template <typename T>
  static ASIO_CONSTEXPR
  typename traits::static_query<T, untracked_t>::result_type
  static_query(
      typename enable_if<
        !traits::query_static_constexpr_member<
            T, outstanding_work_t>::is_valid
          && !traits::query_member<T, outstanding_work_t>::is_valid
          && traits::static_query<T, tracked_t>::is_valid
      >::type* = 0) ASIO_NOEXCEPT
  {
    return traits::static_query<T, untracked_t>::value();
  }

  template <typename T>
  static ASIO_CONSTEXPR
  typename traits::static_query<T, tracked_t>::result_type
  static_query(
      typename enable_if<
        !traits::query_static_constexpr_member<
            T, outstanding_work_t>::is_valid
          && !traits::query_member<T, outstanding_work_t>::is_valid
          && !traits::static_query<T, untracked_t>::is_valid
          && traits::static_query<T, tracked_t>::is_valid
      >::type* = 0) ASIO_NOEXCEPT
  {
    return traits::static_query<T, tracked_t>::value();
  }

  template <typename E,
      typename T = decltype(outstanding_work_t::static_query<E>())>
  static ASIO_CONSTEXPR const T static_query_v
    = outstanding_work_t::static_query<E>();
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

  friend ASIO_CONSTEXPR bool operator==(
      const outstanding_work_t& a, const outstanding_work_t& b)
  {
    return a.value_ == b.value_;
  }

  friend ASIO_CONSTEXPR bool operator!=(
      const outstanding_work_t& a, const outstanding_work_t& b)
  {
    return a.value_ != b.value_;
  }

  struct convertible_from_outstanding_work_t
  {
    ASIO_CONSTEXPR convertible_from_outstanding_work_t(outstanding_work_t)
    {
    }
  };

  template <typename Executor>
  friend ASIO_CONSTEXPR outstanding_work_t query(
      const Executor& ex, convertible_from_outstanding_work_t,
      typename enable_if<
        can_query<Executor, untracked_t>::value
      >::type* = 0)
#if !defined(__clang__) // Clang crashes if noexcept is used here.
    ASIO_NOEXCEPT_IF((is_nothrow_query<Executor, untracked_t>::value))
#endif // !defined(__clang__)
  {
    return asio::query(ex, untracked_t());
  }

  template <typename Executor>
  friend ASIO_CONSTEXPR outstanding_work_t query(
      const Executor& ex, convertible_from_outstanding_work_t,
      typename enable_if<
        !can_query<Executor, untracked_t>::value
          && can_query<Executor, tracked_t>::value
      >::type* = 0)
#if !defined(__clang__) // Clang crashes if noexcept is used here.
    ASIO_NOEXCEPT_IF((is_nothrow_query<Executor, tracked_t>::value))
#endif // !defined(__clang__)
  {
    return asio::query(ex, tracked_t());
  }

  ASIO_STATIC_CONSTEXPR_DEFAULT_INIT(untracked_t, untracked);
  ASIO_STATIC_CONSTEXPR_DEFAULT_INIT(tracked_t, tracked);

#if !defined(ASIO_HAS_CONSTEXPR)
  static const outstanding_work_t instance;
#endif // !defined(ASIO_HAS_CONSTEXPR)

private:
  int value_;
};

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_VARIABLE_TEMPLATES)
template <int I> template <typename E, typename T>
const T outstanding_work_t<I>::static_query_v;
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

#if !defined(ASIO_HAS_CONSTEXPR)
template <int I>
const outstanding_work_t<I> outstanding_work_t<I>::instance;
#endif

template <int I>
const typename outstanding_work_t<I>::untracked_t
outstanding_work_t<I>::untracked;

template <int I>
const typename outstanding_work_t<I>::tracked_t outstanding_work_t<I>::tracked;

namespace outstanding_work {

template <int I = 0>
struct untracked_t
{
#if defined(ASIO_HAS_VARIABLE_TEMPLATES)
  template <typename T>
  ASIO_STATIC_CONSTEXPR(bool,
    is_applicable_property_v = is_executor<T>::value);
#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

  ASIO_STATIC_CONSTEXPR(bool, is_requirable = true);
  ASIO_STATIC_CONSTEXPR(bool, is_preferable = true);
  typedef outstanding_work_t<I> polymorphic_query_result_type;

  ASIO_CONSTEXPR untracked_t()
  {
  }

#if defined(ASIO_HAS_DECLTYPE) \
&& defined(ASIO_HAS_NOEXCEPT) \
&& defined(ASIO_HAS_VARIABLE_TEMPLATES)
  template <typename T>
  static ASIO_CONSTEXPR
  typename traits::query_static_constexpr_member<T, untracked_t>::result_type
  static_query()
    ASIO_NOEXCEPT_IF((
      traits::query_static_constexpr_member<T, untracked_t>::is_noexcept))
  {
    return traits::query_static_constexpr_member<T, untracked_t>::value();
  }

  template <typename T>
  static ASIO_CONSTEXPR untracked_t static_query(
      typename enable_if<
        !traits::query_static_constexpr_member<T, untracked_t>::is_valid
          && !traits::query_member<T, untracked_t>::is_valid
          && !traits::query_free<T, untracked_t>::is_valid
          && !can_query<T, tracked_t<I> >::value
      >::type* = 0) ASIO_NOEXCEPT
  {
    return untracked_t();
  }

  template <typename E, typename T = decltype(untracked_t::static_query<E>())>
  static ASIO_CONSTEXPR const T static_query_v
    = untracked_t::static_query<E>();
#endif // defined(ASIO_HAS_DECLTYPE)
     //   && defined(ASIO_HAS_NOEXCEPT)
     //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

  static ASIO_CONSTEXPR outstanding_work_t<I> value()
  {
    return untracked_t();
  }

  friend ASIO_CONSTEXPR bool operator==(
      const untracked_t&, const untracked_t&)
  {
    return true;
  }

  friend ASIO_CONSTEXPR bool operator!=(
      const untracked_t&, const untracked_t&)
  {
    return false;
  }
};

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_VARIABLE_TEMPLATES)
template <int I> template <typename E, typename T>
const T untracked_t<I>::static_query_v;
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <int I = 0>
struct tracked_t
{
#if defined(ASIO_HAS_VARIABLE_TEMPLATES)
  template <typename T>
  ASIO_STATIC_CONSTEXPR(bool,
    is_applicable_property_v = is_executor<T>::value);
#endif // defined(ASIO_HAS_VARIABLE_TEMPLATES)

  ASIO_STATIC_CONSTEXPR(bool, is_requirable = true);
  ASIO_STATIC_CONSTEXPR(bool, is_preferable = true);
  typedef outstanding_work_t<I> polymorphic_query_result_type;

  ASIO_CONSTEXPR tracked_t()
  {
  }

#if defined(ASIO_HAS_DECLTYPE) \
&& defined(ASIO_HAS_NOEXCEPT) \
&& defined(ASIO_HAS_VARIABLE_TEMPLATES)
  template <typename T>
  static ASIO_CONSTEXPR
  typename traits::query_static_constexpr_member<T, tracked_t>::result_type
  static_query()
    ASIO_NOEXCEPT_IF((
      traits::query_static_constexpr_member<T, tracked_t>::is_noexcept))
  {
    return traits::query_static_constexpr_member<T, tracked_t>::value();
  }

  template <typename E, typename T = decltype(tracked_t::static_query<E>())>
  static ASIO_CONSTEXPR const T static_query_v
    = tracked_t::static_query<E>();
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

  static ASIO_CONSTEXPR outstanding_work_t<I> value()
  {
    return tracked_t();
  }

  friend ASIO_CONSTEXPR bool operator==(
      const tracked_t&, const tracked_t&)
  {
    return true;
  }

  friend ASIO_CONSTEXPR bool operator!=(
      const tracked_t&, const tracked_t&)
  {
    return false;
  }
};

#if defined(ASIO_HAS_DECLTYPE) \
  && defined(ASIO_HAS_NOEXCEPT) \
  && defined(ASIO_HAS_VARIABLE_TEMPLATES)
template <int I> template <typename E, typename T>
const T tracked_t<I>::static_query_v;
#endif // defined(ASIO_HAS_DECLTYPE)
       //   && defined(ASIO_HAS_NOEXCEPT)
       //   && defined(ASIO_HAS_VARIABLE_TEMPLATES)

} // namespace outstanding_work
} // namespace detail

typedef detail::outstanding_work_t<> outstanding_work_t;

#if defined(ASIO_HAS_CONSTEXPR) || defined(GENERATING_DOCUMENTATION)
constexpr outstanding_work_t outstanding_work;
#else // defined(ASIO_HAS_CONSTEXPR) || defined(GENERATING_DOCUMENTATION)
namespace { static const outstanding_work_t&
  outstanding_work = outstanding_work_t::instance; }
#endif

} // namespace execution

#if !defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename T>
struct is_applicable_property<T, execution::outstanding_work_t>
  : execution::is_executor<T>
{
};

template <typename T>
struct is_applicable_property<T, execution::outstanding_work_t::untracked_t>
  : execution::is_executor<T>
{
};

template <typename T>
struct is_applicable_property<T, execution::outstanding_work_t::tracked_t>
  : execution::is_executor<T>
{
};

#endif // !defined(ASIO_HAS_VARIABLE_TEMPLATES)

namespace traits {

#if !defined(ASIO_HAS_DECLTYPE) \
  || !defined(ASIO_HAS_NOEXCEPT)

template <typename T>
struct query_free_default<T, execution::outstanding_work_t,
  typename enable_if<
    can_query<T, execution::outstanding_work_t::untracked_t>::value
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept =
    (is_nothrow_query<T, execution::outstanding_work_t::untracked_t>::value));

  typedef execution::outstanding_work_t result_type;
};

template <typename T>
struct query_free_default<T, execution::outstanding_work_t,
  typename enable_if<
    !can_query<T, execution::outstanding_work_t::untracked_t>::value
      && can_query<T, execution::outstanding_work_t::tracked_t>::value
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept =
    (is_nothrow_query<T, execution::outstanding_work_t::tracked_t>::value));

  typedef execution::outstanding_work_t result_type;
};

#endif // !defined(ASIO_HAS_DECLTYPE)
       //   || !defined(ASIO_HAS_NOEXCEPT)

#if !defined(ASIO_HAS_DECLTYPE) \
  || !defined(ASIO_HAS_NOEXCEPT) \
  || !defined(ASIO_HAS_VARIABLE_TEMPLATES)

template <typename T>
struct static_query<T, execution::outstanding_work_t,
  typename enable_if<
    traits::query_static_constexpr_member<T,
      execution::outstanding_work_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);

  typedef typename traits::query_static_constexpr_member<T,
    execution::outstanding_work_t>::result_type result_type;

  static ASIO_CONSTEXPR result_type value()
  {
    return traits::query_static_constexpr_member<T,
      execution::outstanding_work_t>::value();
  }
};

template <typename T>
struct static_query<T, execution::outstanding_work_t,
  typename enable_if<
    !traits::query_static_constexpr_member<T,
        execution::outstanding_work_t>::is_valid
      && !traits::query_member<T, execution::outstanding_work_t>::is_valid
      && traits::static_query<T,
          execution::outstanding_work_t::untracked_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);

  typedef typename traits::static_query<T,
    execution::outstanding_work_t::untracked_t>::result_type result_type;

  static ASIO_CONSTEXPR result_type value()
  {
    return traits::static_query<T,
        execution::outstanding_work_t::untracked_t>::value();
  }
};

template <typename T>
struct static_query<T, execution::outstanding_work_t,
  typename enable_if<
    !traits::query_static_constexpr_member<T,
        execution::outstanding_work_t>::is_valid
      && !traits::query_member<T, execution::outstanding_work_t>::is_valid
      && !traits::static_query<T,
          execution::outstanding_work_t::untracked_t>::is_valid
      && traits::static_query<T,
          execution::outstanding_work_t::tracked_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);

  typedef typename traits::static_query<T,
    execution::outstanding_work_t::tracked_t>::result_type result_type;

  static ASIO_CONSTEXPR result_type value()
  {
    return traits::static_query<T,
        execution::outstanding_work_t::tracked_t>::value();
  }
};

template <typename T>
struct static_query<T, execution::outstanding_work_t::untracked_t,
  typename enable_if<
    traits::query_static_constexpr_member<T,
      execution::outstanding_work_t::untracked_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);

  typedef typename traits::query_static_constexpr_member<T,
    execution::outstanding_work_t::untracked_t>::result_type result_type;

  static ASIO_CONSTEXPR result_type value()
  {
    return traits::query_static_constexpr_member<T,
      execution::outstanding_work_t::untracked_t>::value();
  }
};

template <typename T>
struct static_query<T, execution::outstanding_work_t::untracked_t,
  typename enable_if<
    !traits::query_static_constexpr_member<T,
        execution::outstanding_work_t::untracked_t>::is_valid
      && !traits::query_member<T,
          execution::outstanding_work_t::untracked_t>::is_valid
      && !traits::query_free<T,
          execution::outstanding_work_t::untracked_t>::is_valid
      && !can_query<T, execution::outstanding_work_t::tracked_t>::value
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);

  typedef execution::outstanding_work_t::untracked_t result_type;

  static ASIO_CONSTEXPR result_type value()
  {
    return result_type();
  }
};

template <typename T>
struct static_query<T, execution::outstanding_work_t::tracked_t,
  typename enable_if<
    traits::query_static_constexpr_member<T,
      execution::outstanding_work_t::tracked_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = true);

  typedef typename traits::query_static_constexpr_member<T,
    execution::outstanding_work_t::tracked_t>::result_type result_type;

  static ASIO_CONSTEXPR result_type value()
  {
    return traits::query_static_constexpr_member<T,
      execution::outstanding_work_t::tracked_t>::value();
  }
};

template <typename T>
struct static_require<T, execution::outstanding_work_t::untracked_t,
  typename enable_if<
    static_query<T, execution::outstanding_work_t::untracked_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid =
    (is_same<typename static_query<T,
      execution::outstanding_work_t::untracked_t>::result_type,
        execution::outstanding_work_t::untracked_t>::value));
};

template <typename T>
struct static_require<T, execution::outstanding_work_t::tracked_t,
  typename enable_if<
    static_query<T, execution::outstanding_work_t::tracked_t>::is_valid
  >::type>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid =
    (is_same<typename static_query<T,
      execution::outstanding_work_t::tracked_t>::result_type,
        execution::outstanding_work_t::tracked_t>::value));
};

#endif // !defined(ASIO_HAS_DECLTYPE)
       //   || !defined(ASIO_HAS_NOEXCEPT)
       //   || !defined(ASIO_HAS_VARIABLE_TEMPLATES)

} // namespace traits
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_EXECUTION_OUTSTANDING_WORK_HPP
