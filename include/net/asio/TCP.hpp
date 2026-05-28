// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NET_ASIO_TCP
#define NET_ASIO_TCP

#include <net/asio/stream.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace net::IP::TCP {

    using stream = asio::byte_stream<asio::ip::tcp::socket>;

}

#endif

