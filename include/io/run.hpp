// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef IO_RUN
#define IO_RUN

#include <net/asio/stream.hpp>

#include <boost/version.hpp>
#include <boost/process.hpp>

namespace io {

    using string = std::string;
    using string_view = std::string_view;
    template <typename X> using ptr = std::shared_ptr<X>;
    template <typename X> using cross = data::cross<X>;
    template <typename X> using awaitable = data::awaitable<X>;

    namespace bp = boost::process;

    struct process_result {
        int exit_code;
    };

    // run an external program.
    awaitable<process_result> execute (data::exec, cross<string> command);

    struct running_process;

    // launch asynchronously
    running_process run (data::exec, cross<string> command);

    struct running_process {

        running_process (data::exec, cross<string> command);

        using pipe_out = net::asio::char_out_stream<boost::asio::writable_pipe>;
        using pipe_in = net::asio::char_in_stream<boost::asio::readable_pipe>;

        // stdin of child
        pipe_out In;

        // stdout of child
        pipe_in Out;

        // stderr of child
        pipe_in Err;

        // await process completion
        awaitable<process_result> result ();

        boost::process::v2::process Child;

        // TODO need move semantics and copy is not allowed.
    };

    // convenience wrapper
    awaitable<process_result> inline execute (data::exec x, cross<string> command) {
        // if we compress this in one line, then
        // p becomes a temporary which might be
        // deleted too early.
        auto p = run (x, command);
        co_return co_await p.result ();
    }

    running_process run (data::exec x, cross<string> command) {
        return running_process {x, command};
    }

}

#endif

