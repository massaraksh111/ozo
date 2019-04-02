#pragma once

#include <ozo/type_traits.h>
#include <ozo/optional.h>
#include <ozo/io/send.h>
#include <ozo/detail/functional.h>

namespace ozo {

template <>
struct is_nullable<__OZO_NULLOPT_T> : std::true_type {};

template <>
struct unwrap_impl<__OZO_NULLOPT_T> : detail::functional::forward {};

template <>
struct is_null_impl<__OZO_NULLOPT_T> : detail::functional::always_true {};

template <>
struct send_impl<__OZO_NULLOPT_T> {
    template <typename M, typename T>
    static ostream& apply(ostream& out, M&&, T&&) { return out;}
};

} // namespace ozo

OZO_PG_DEFINE_TYPE(__OZO_NULLOPT_T, "null", null_oid, decltype(null_state_size))
