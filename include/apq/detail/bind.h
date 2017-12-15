#pragma once

#include <tuple>
#include <utility>
#include <boost/asio/handler_invoke_hook.hpp>

namespace libapq {
namespace detail {

/**
 * This binder preserves handler context just like
 * boost::asio::detail::bind_handler does. It is
 * needed because in other case you will loose the handler
 * context. 
 * 
 * For more information see the link below:
 *
 *     https://stackoverflow.com/questions/32857101/when-to-use-asio-handler-invoke
 */
template <typename Handler, typename Tuple>
struct binder {
    Handler handler_;
    Tuple args_;

    binder(Handler handler, Tuple&& args)
    : handler_(std::move(handler)), args_(std::move(args)) {}

    auto operator() () { return std::apply(handler_, args_); }

    template <typename Function>
    friend void asio_handler_invoke(Function&& f, binder* ctx) {
        using ::boost::asio::asio_handler_invoke;
        asio_handler_invoke(std::forward<Function>(f), std::addressof(ctx->handler_));
    }
};

template <typename Handler, typename ... Args>
inline auto bind(Handler&& h, Args&& ... args) {
    return binder<std::decay_t<Handler>, 
                std::decay_t<decltype(std::make_tuple(std::forward<Args>(args)...))>> (
        std::forward<Handler>(h), std::make_tuple(std::forward<Args>(args)...));
}

} // namespace detail
} // namespace libapq