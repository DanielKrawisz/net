// Copyright (c) 2021-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NET_ASIO_STREAM
#define NET_ASIO_STREAM

#include <boost/asio.hpp>

#include <data/bytes.hpp>
#include <data/string.hpp>

#include <net/stream.hpp>

// this page has types which convert asio::streams into net::streams.

namespace net::asio {
    using namespace boost::asio;
    using error = boost::system::error_code;

    template <typename const_buffer_sequence>
    concept ConstBufferSequence = std::copy_constructible<const_buffer_sequence> && std::destructible<const_buffer_sequence> &&
        requires (const const_buffer_sequence x) {
            { buffer_sequence_begin (x) };
            { buffer_sequence_end (x) };
        };

    template <typename async_write_stream, typename const_buffer_sequence = const_buffer>
    concept AsyncWriteStream = ConstBufferSequence<const_buffer_sequence> &&
        requires (async_write_stream stream) {
            { stream.get_executor () };
        } && requires (async_write_stream stream, const_buffer_sequence bf) {
            { stream.async_write_some (bf, use_awaitable) } -> data::Same<awaitable<size_t>>;
        };

    template <typename async_read_stream, typename mutable_buffer_sequence>
    concept AsyncReadStream = requires (async_read_stream stream, mutable_buffer_sequence m) {
            { stream.async_read_some (buffer (m), use_awaitable) } -> data::Same<awaitable<size_t>>;
        };

    template <typename X, typename bytes>
    requires AsyncReadStream<X, bytes> class in_stream;

    template <AsyncWriteStream X, typename byte_slice> class out_stream;

    template <AsyncWriteStream X, typename bytes, typename byte_slice>
    requires AsyncReadStream<X, bytes> class stream;

    template <typename X> using byte_in_stream = in_stream<X, bytes>;
    template <typename X> using char_in_stream = in_stream<X, string>;

    template <typename X> using byte_out_stream = out_stream<X, byte_slice>;
    template <typename X> using char_out_stream = out_stream<X, string_view>;

    template <typename X> using byte_stream = stream<X, bytes, byte_slice>;
    template <typename X> using char_stream = stream<X, string, string_view>;

    // an exception that is thrown upon generation of an asio error.
    struct exception : data::exception {
        error Error;
        exception (error ec): Error {ec} {
            *this << "Operation failed because: " << ec;
        }
    };

    template <AsyncWriteStream X, typename bytes, typename byte_slice>
    requires AsyncReadStream<X, bytes>
    class stream final : public net::stream<bytes, byte_slice> {

        ptr<X> Stream;

        // TODO I don't think we need this.
        close_handler OnClose;

        bool Closed;

        constexpr static const int buffer_size = 4096;
        bytes Buffer;

    public:
        stream (ptr<X> socket, close_handler on_close = [] () { return; }) :
            Stream {socket}, OnClose {on_close}, Closed {!Stream->is_open ()}, Buffer (buffer_size) {}

        awaitable<void> send (byte_slice x) final override;

        void close () final override;

        bool closed () final override;

        awaitable<bytes> receive () final override;

        ~stream () {
            close ();
        }
    };

    template <AsyncWriteStream X, typename bytes, typename byte_slice>
    requires AsyncReadStream<X, bytes>
    awaitable<void> stream<X, bytes, byte_slice>::send (byte_slice x) {
        error ec;
        std::size_t n = co_await async_write (
            *Stream,
            buffer (x.data (), x.size ()),
            asio::redirect_error (asio::use_awaitable, ec)
        );

        if (ec) {
            throw exception {ec};
            close ();
        }
    }

    template <AsyncWriteStream X, typename bytes, typename byte_slice>
    requires AsyncReadStream<X, bytes>
    void stream<X, bytes, byte_slice>::close () {
        if (Closed) return;
        Closed = true;
        Stream->close ();
        OnClose ();
    }

    template <AsyncWriteStream X, typename bytes, typename byte_slice>
    requires AsyncReadStream<X, bytes>
    bool stream<X, bytes, byte_slice>::closed () {
        bool closed = !Stream->is_open ();
        if (closed && !Closed) {
            Closed = true;
            OnClose ();
        }
        return closed;
    }

    template <AsyncWriteStream X, typename bytes, typename byte_slice>
    requires AsyncReadStream<X, bytes>
    awaitable<bytes> stream<X, bytes, byte_slice>::receive () {
        error ec;
        // if there is no error, bytes_transferred should always be greater than zero.
        // if bytes_read == buffer_size, we cannot know if there are more bytes on their
        // way. Reconstructing a complete message has to come at a higher level.
        size_t bytes_read = co_await Stream->async_read_some (buffer (Buffer.data (), Buffer.size ()), redirect_error (use_awaitable, ec));

        if (ec) {
            close ();
            throw exception {ec};
        }

        bytes b (bytes_read);
        std::copy (Buffer.begin (), Buffer.begin () + bytes_read, b.begin ());
        co_return b;
    }

}

#endif

