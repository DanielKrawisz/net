// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <io/run.hpp>

namespace io {

    running_process::running_process (data::exec io, cross<string> command):
        In {std::make_shared<boost::asio::writable_pipe> (io)},
        Out {std::make_shared<boost::asio::readable_pipe> (io)},
        Err {std::make_shared<boost::asio::readable_pipe> (io)},
        Child {
            io,
            command[0],
            drop (command, 1),
            boost::process::v2::process_stdio {
                *In.Stream,
                *Out.Stream,
                *Err.Stream}} {}
/*
    // run an external command with standard in and standard out connected.
    void run (boost::asio::io_context &io, string command, error_handler err_handler, interaction i, close_handler close) {

        ptr<bp::async_pipe> in {new pipe {io}};
        ptr<bp::async_pipe> out {new pipe {io}};
        ptr<bp::async_pipe> err {new pipe {io}};

        ptr<bp::child> child { new bp::child (command, bp::std_out > *out, bp::std_err > *err, bp::std_in < *in) };

        ptr<socket> In { new socket {in, close}};
        ptr<socket> Out { new socket {out, close}};
        ptr<socket> Err { new socket {err, close}};

        ptr<process> p {new process {std::move (child), In, err_handler}};

        auto handle = i (p);

        net::async::wait_for_message<string_view, error_code> (Out, [handle] (string_view x) -> void {
            handle->read_out (x);
        }, err_handler);

        net::async::wait_for_message<string_view, error_code> (Err, [handle] (string_view x) -> void {
            handle->read_err (x);
        }, err_handler);

    }*/

}


