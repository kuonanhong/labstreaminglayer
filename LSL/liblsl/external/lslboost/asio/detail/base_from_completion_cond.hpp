//
// detail/base_from_completion_cond.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.lslboost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_BASE_FROM_COMPLETION_COND_HPP
#define BOOST_ASIO_DETAIL_BASE_FROM_COMPLETION_COND_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <lslboost/asio/detail/config.hpp>
#include <lslboost/asio/completion_condition.hpp>

#include <lslboost/asio/detail/push_options.hpp>

namespace lslboost {
namespace asio {
namespace detail {

template <typename CompletionCondition>
class base_from_completion_cond
{
protected:
  explicit base_from_completion_cond(CompletionCondition completion_condition)
    : completion_condition_(completion_condition)
  {
  }

  std::size_t check_for_completion(
      const lslboost::system::error_code& ec,
      std::size_t total_transferred)
  {
    return detail::adapt_completion_condition_result(
        completion_condition_(ec, total_transferred));
  }

private:
  CompletionCondition completion_condition_;
};

template <>
class base_from_completion_cond<transfer_all_t>
{
protected:
  explicit base_from_completion_cond(transfer_all_t)
  {
  }

  static std::size_t check_for_completion(
      const lslboost::system::error_code& ec,
      std::size_t total_transferred)
  {
    return transfer_all_t()(ec, total_transferred);
  }
};

} // namespace detail
} // namespace asio
} // namespace lslboost

#include <lslboost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_BASE_FROM_COMPLETION_COND_HPP
