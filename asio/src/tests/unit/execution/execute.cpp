//
// execute.cpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include "asio/execution/execute.hpp"

#include "asio/execution/invocable_archetype.hpp"
#include "../unit_test.hpp"

#if defined(ASIO_HAS_BOOST_BIND)
# include <boost/bind/bind.hpp>
#else // defined(ASIO_HAS_BOOST_BIND)
# include <functional>
#endif // defined(ASIO_HAS_BOOST_BIND)

namespace exec = asio::execution;

struct no_execute
{
};

struct non_const_member_execute
{
  template <typename F>
  void execute(ASIO_MOVE_ARG(F) f)
  {
    typename asio::decay<F>::type tmp(ASIO_MOVE_CAST(F)(f));
    f();
  }
};

#if !defined(ASIO_HAS_DECLTYPE) \
  || !defined(ASIO_HAS_NOEXCEPT)

namespace asio {
namespace traits {

template <typename F>
struct execute_member<non_const_member_execute, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

template <typename F>
struct execute_member<const non_const_member_execute, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DECLTYPE)
       //   || !defined(ASIO_HAS_NOEXCEPT)

struct const_member_execute
{
  template <typename F>
  void execute(ASIO_MOVE_ARG(F) f) const
  {
    typename asio::decay<F>::type tmp(ASIO_MOVE_CAST(F)(f));
    f();
  }
};

#if !defined(ASIO_HAS_DECLTYPE) \
  || !defined(ASIO_HAS_NOEXCEPT)

namespace asio {
namespace traits {

template <typename F>
struct execute_member<const_member_execute, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DECLTYPE)
       //   || !defined(ASIO_HAS_NOEXCEPT)

struct free_execute_non_const_executor
{
  template <typename F>
  friend void execute(free_execute_non_const_executor&,
      ASIO_MOVE_ARG(F) f)
  {
    typename asio::decay<F>::type tmp(ASIO_MOVE_CAST(F)(f));
    f();
  }
};

#if !defined(ASIO_HAS_DECLTYPE) \
  || !defined(ASIO_HAS_NOEXCEPT)

namespace asio {
namespace traits {

template <typename F>
struct execute_free<free_execute_non_const_executor, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

template <typename F>
struct execute_free<const free_execute_non_const_executor, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = false);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DECLTYPE)
       //   || !defined(ASIO_HAS_NOEXCEPT)

struct free_execute_const_executor
{
  template <typename F>
  friend void execute(const free_execute_const_executor&,
      ASIO_MOVE_ARG(F) f)
  {
    typename asio::decay<F>::type tmp(ASIO_MOVE_CAST(F)(f));
    f();
  }
};

#if !defined(ASIO_HAS_DECLTYPE) \
  || !defined(ASIO_HAS_NOEXCEPT)

namespace asio {
namespace traits {

template <typename F>
struct execute_free<free_execute_const_executor, F>
{
  ASIO_STATIC_CONSTEXPR(bool, is_valid = true);
  ASIO_STATIC_CONSTEXPR(bool, is_noexcept = false);
  typedef void result_type;
};

} // namespace traits
} // namespace asio

#endif // !defined(ASIO_HAS_DECLTYPE)
       //   || !defined(ASIO_HAS_NOEXCEPT)

void test_can_execute()
{
  ASIO_CONSTEXPR bool b1 = exec::can_execute<
      no_execute&, exec::invocable_archetype>::value;
  ASIO_CHECK(b1 == false);

  ASIO_CONSTEXPR bool b2 = exec::can_execute<
      const no_execute&, exec::invocable_archetype>::value;
  ASIO_CHECK(b2 == false);

  ASIO_CONSTEXPR bool b3 = exec::can_execute<
      non_const_member_execute&, exec::invocable_archetype>::value;
  ASIO_CHECK(b3 == true);

  ASIO_CONSTEXPR bool b4 = exec::can_execute<
      const non_const_member_execute&, exec::invocable_archetype>::value;
  ASIO_CHECK(b4 == false);

  ASIO_CONSTEXPR bool b5 = exec::can_execute<
      const_member_execute&, exec::invocable_archetype>::value;
  ASIO_CHECK(b5 == true);

  ASIO_CONSTEXPR bool b6 = exec::can_execute<
      const const_member_execute&, exec::invocable_archetype>::value;
  ASIO_CHECK(b6 == true);

  ASIO_CONSTEXPR bool b7 = exec::can_execute<
      free_execute_non_const_executor&, exec::invocable_archetype>::value;
  ASIO_CHECK(b7 == true);

  ASIO_CONSTEXPR bool b8 = exec::can_execute<
      const free_execute_non_const_executor&, exec::invocable_archetype>::value;
  ASIO_CHECK(b8 == false);

  ASIO_CONSTEXPR bool b9 = exec::can_execute<
      free_execute_const_executor&, exec::invocable_archetype>::value;
  ASIO_CHECK(b9 == true);

  ASIO_CONSTEXPR bool b10 = exec::can_execute<
      const free_execute_const_executor&, exec::invocable_archetype>::value;
  ASIO_CHECK(b10 == true);
}

void increment(int* count)
{
  ++(*count);
}

void test_execute()
{
#if defined(ASIO_HAS_BOOST_BIND)
  namespace bindns = boost;
#else // defined(ASIO_HAS_BOOST_BIND)
  namespace bindns = std;
#endif // defined(ASIO_HAS_BOOST_BIND)

  int count = 0;
  non_const_member_execute ex1;
  exec::execute(ex1, bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  const_member_execute ex2;
  exec::execute(ex2, bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  const const_member_execute ex3;
  exec::execute(ex3, bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  exec::execute(const_member_execute(), bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  free_execute_non_const_executor ex4;
  exec::execute(ex4, bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  free_execute_const_executor ex5;
  exec::execute(ex5, bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  const free_execute_const_executor ex6;
  exec::execute(ex6, bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);

  count = 0;
  exec::execute(free_execute_const_executor(),
      bindns::bind(&increment, &count));
  ASIO_CHECK(count == 1);
}

ASIO_TEST_SUITE
(
  "blocking",
  ASIO_TEST_CASE(test_can_execute)
  ASIO_TEST_CASE(test_execute)
)
