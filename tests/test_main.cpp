#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../inc/synth.hpp"

TEST_CASE( "Add function works" ) {
    REQUIRE( Synth::add_function(4, 5) == 9 );
}
