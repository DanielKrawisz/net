// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#include <io/run.hpp>

namespace io {

    run::run (data::exec io, cross<string> command):
        In {std::make_shared<asio::writable_pipe> (io)},
        Out {std::make_shared<asio::readable_pipe> (io)},
        Err {std::make_shared<asio::readable_pipe> (io)},
        Child {
            io,
            command[0],
            drop (command, 1),
            boost::process::v2::process_stdio {
                *In.Stream,
                *Out.Stream,
                *Err.Stream}} {}

}


