#include <catch2/catch_test_macros.hpp>

extern "C" {
    #include "../src/write.h"
}

TEST_CASE( "Calculations are completed", "[calc]" ) {
    REQUIRE( calc(0, 0) == 0 );
    REQUIRE( calc(-1, 1) == 0 );
}

TEST_CASE( "Data can be written", "[write]" ) {
    REQUIRE( write(1, "hello\n", 6) == 0 );
}