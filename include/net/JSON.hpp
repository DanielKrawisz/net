// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef NET_JSON
#define NET_JSON

#include <nlohmann/json.hpp>

#include <net/serialized.hpp>

namespace net {
    using JSON = nlohmann::json;

    open<JSON> inline JSON_stream (open<std::string, string_view> Open);

    // JSON line stream means that we insert a new line at the end of every JSON type.
    open<JSON> inline JSON_line_stream (open<std::string, string_view> Open);

    // throws JSON::exception if the format is bad.
    ptr<writer<char>> JSON_line_parser (handler<const JSON &>);

    open<JSON> inline JSON_stream (open<std::string, string_view> Open) {
        return [Open] (close_handler on_close, interaction<JSON> receiver) -> awaitable<ptr<out_stream<const JSON &>>> {
            function<std::string (const JSON &)> generator =
                [] (const JSON &j) -> std::string {
                    return j.dump ();
                };

            function<JSON (string_view)> reader =
                [] (string_view x) -> JSON {
                    auto j = JSON::parse (x);
                    return j;
                };

            return serialized_stream_read<JSON> (Open, generator, reader) (on_close, receiver);
        };
    }

    // JSON line stream means that we insert a new line at the end of every JSON type.
    open<JSON> inline JSON_line_stream (open<std::string, string_view> Open) {
        return [Open] (close_handler on_close, interaction<JSON> receiver) -> awaitable<ptr<out_stream<const JSON &>>> {
            function<std::string (const JSON &)> generator =
                [] (const JSON &j) -> std::string {
                    return string::write (j.dump (), "\n");
                };

            function<ptr<writer<char>> (handler<const JSON &>)> parser =
                [] (handler<const JSON &> h) -> ptr<writer<char>> {
                    return JSON_line_parser (h);
                };

            return serialized_stream_parsed<JSON> (Open, generator, parser) (on_close, receiver);
        };
    }
    
}

#endif
