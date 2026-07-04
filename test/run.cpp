// Copyright (c) 2026 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <io/run.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

namespace io {

    // test a nonexistent script.
    TEST (Run, Nonexistent) {
        // TODO
    }

    TEST (Run, EchoArg) {
        EXPECT_EQ ((data::synced (execute, cross<string> {"run/echo_arg.sh", "0"})), 0);

        EXPECT_EQ ((data::synced (execute, cross<string> {"run/echo_arg.sh", "1"})), 1);
    }

}

