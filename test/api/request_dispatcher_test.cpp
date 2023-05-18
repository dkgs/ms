#include "api/request_dispatcher.hpp"

#include <catch2/catch.hpp>

struct response_mock
{
};

struct request_handler_mock
{
    bool
    can_handle(const std::vector<std::string> & route_split)
    {
        return route_split.size() >= 1 && route_split.at(0) == "simple";
    }

    std::variant<response_mock>
    handle(const std::vector<std::string> & route_split, const boost::beast::http::request<boost::beast::http::string_body> & request)
    {
        return {};
    }
    
};

struct response_serializer_mock
{
    boost::beast::http::response<boost::beast::http::string_body>
    operator()(
        const boost::beast::http::request<boost::beast::http::string_body> & request,
        const auto & response
    )
    {
        boost::beast::http::response<boost::beast::http::string_body> http_response {boost::beast::http::status::ok, request.version()};
        return http_response;
    }
};

SCENARIO("Test of the request dispatcher")
{
    api::request_dispatcher<response_serializer_mock, request_handler_mock> req_dis {
        {},
        {
            request_handler_mock{}
        }
    };
    WHEN("A known route request arrives")
    {
        boost::beast::http::request<boost::beast::http::string_body> request {boost::beast::http::verb::get, "/simple", 10};
        THEN("It is handled by its specific handler")
        {
            auto response = req_dis.dispatch(request);
            REQUIRE( response.result_int() == 200 );
        }
    }
    WHEN("An unknown request route arrives")
    {
        boost::beast::http::request<boost::beast::http::string_body> request {boost::beast::http::verb::get, "/unknown", 10};
        THEN ("A 404 response")
        {
            auto response = req_dis.dispatch(request);
            REQUIRE( response.result_int() == 404 );
        }
    }
}
