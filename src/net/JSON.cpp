#include <net/JSON.hpp>

#include <sstream>

namespace net {

    ptr<writer<char>> JSON_line_parser (handler<const JSON::exception &> errors, handler<const JSON &> handle) {
        struct parser : writer<char> {
            handler<const JSON &> Handler;
            handler<const JSON::exception &> ErrorHandler;

            std::stringstream Stream;

            parser (handler<const JSON &> handle, handler<const JSON::exception &> errors):
                Handler {handle}, ErrorHandler {errors}, Stream {} {}

            void write (const char *data, size_t size) final override {
                int last_new_line = 0;

                for (int i = 0; i < size; i++) if (data[i] == '\n') {
                    Stream.write (data + last_new_line, i);

                    try {
                        Handler (JSON::parse (Stream.str ()));
                    } catch (const JSON::exception &x) {
                        ErrorHandler (x);
                    }

                    Stream = std::stringstream {};
                    last_new_line = i + 1;
                }

                Stream.write ((const char*) (data + last_new_line), size - last_new_line);
            }

            virtual ~parser () noexcept (false) {}
        };

        return std::static_pointer_cast<writer<char>>
            (std::make_shared<parser> (handle, errors));
    }
}

