//
// as_single.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_AS_SINGLE_HPP
#define ASIO_AS_SINGLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"
#include "asio/error_code.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {

/// Completion token type used to specify that an error produced by an
/// asynchronous operation is captured to an error_code variable.
/**
 * The as_single_t class is used to indicate that any error_code produced
 * by an asynchronous operation is captured to a specified variable.
 */
template <typename CompletionToken>
class as_single_t
{
public:
  /// Default constructor.
  /**
   * This constructor is only valid if the underlying completion token is
   * default constructible.
   */
  ASIO_CONSTEXPR as_single_t()
    : token_()
  {
  }

  /// Constructor. 
  template <typename T>
  ASIO_CONSTEXPR explicit as_single_t(
      ASIO_MOVE_ARG(T) completion_token)
    : token_(ASIO_MOVE_CAST(T)(completion_token))
  {
  }

  /// Adapts an executor to add the @c as_single_t completion token as the
  /// default.
  template <typename InnerExecutor>
  struct executor_with_default : InnerExecutor
  {
    /// Specify @c as_single_t as the default completion token type.
    typedef as_single_t default_completion_token_type;

    /// Construct the adapted executor from the inner executor type.
    executor_with_default(const InnerExecutor& ex) ASIO_NOEXCEPT
      : InnerExecutor(ex)
    {
    }
  };

  /// Type alias to adapt an I/O object to use @c as_single_t as its
  /// default completion token type.
#if defined(ASIO_HAS_ALIAS_TEMPLATES) \
  || defined(GENERATING_DOCUMENTATION)
  template <typename T>
  using as_default_on_t = typename T::template rebind_executor<
      executor_with_default<typename T::executor_type> >::other;
#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)
       //   || defined(GENERATING_DOCUMENTATION)

  /// Function helper to adapt an I/O object to use @c as_single_t as its
  /// default completion token type.
  template <typename T>
  static typename decay<T>::type::template rebind_executor<
      executor_with_default<typename decay<T>::type::executor_type>
    >::other
  as_default_on(ASIO_MOVE_ARG(T) object)
  {
    return typename decay<T>::type::template rebind_executor<
        executor_with_default<typename decay<T>::type::executor_type>
      >::other(ASIO_MOVE_CAST(T)(object));
  }

//private:
  CompletionToken token_;
};

/// Create a completion token to capture error_code values to a variable.
template <typename CompletionToken>
inline ASIO_CONSTEXPR as_single_t<typename decay<CompletionToken>::type>
as_single(ASIO_MOVE_ARG(CompletionToken) completion_token)
{
  return as_single_t<typename decay<CompletionToken>::type>(
      ASIO_MOVE_CAST(CompletionToken)(completion_token));
}

} // namespace asio

#include "asio/detail/pop_options.hpp"

#include "asio/impl/as_single.hpp"

#endif // ASIO_AS_SINGLE_HPP
