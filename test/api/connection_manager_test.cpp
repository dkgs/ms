#include "api/connection_manager.hpp"

#include <catch2/catch.hpp>

struct request_dispatcher_mock
{
    boost::beast::http::response<boost::beast::http::string_body>
    dispatch(const boost::beast::http::request<boost::beast::http::string_body> & request)
    {
        return {};
    }
};

SCENARIO("Test of the connection manager")
{

    WHEN("the connection manager is instanciated")
    {
        api::connection_manager<request_dispatcher_mock> cm {{}};
        THEN("it compiles")
        {
            // Because the connection_manager runs synchronous http request
            // we cannot simply launch it and stop it in a unit test
            // it requires more integration testing at this point
            // or a redesign as an asynchronous http request acceptor.
            REQUIRE( true );
        }
    }

}
