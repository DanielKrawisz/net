// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef IO_RUN
#define IO_RUN

#include <net/asio/stream.hpp>

#include <boost/version.hpp>
#include <boost/process.hpp>

namespace io {

    namespace asio = boost::asio;

    using string = std::string;
    using string_view = std::string_view;
    template <typename X> using ptr = std::shared_ptr<X>;
    template <typename X> using cross = data::cross<X>;
    template <typename X> using awaitable = data::awaitable<X>;

    namespace bp = boost::process;

    // run an external program.
    awaitable<int> execute (cross<string> command);

    struct run {

        run (data::exec, cross<string> command);

        using pipe_out = net::asio::char_out_stream<boost::asio::writable_pipe>;
        using pipe_in = net::asio::char_in_stream<boost::asio::readable_pipe>;

        // stdin of child
        pipe_out In;

        // stdout of child
        pipe_in Out;

        // stderr of child
        pipe_in Err;

        // await process completion
        awaitable<int> result ();

        bp::v2::process Child;

        run (const run &) = delete;
        run &operator = (const run &) = delete;

        run (run &&);
        run &operator = (run &&);
    };

    // convenience wrapper
    awaitable<int> inline execute (cross<string> command) {
        // if we compress this in one line, then
        // p becomes a temporary which might be
        // deleted too early.
        auto p = run {co_await asio::this_coro::executor, command};
        co_return co_await p.result ();
    }

}

#endif

