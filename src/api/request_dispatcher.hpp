#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/beast/http.hpp>
#include <boost/url/decode_view.hpp>
#include <variant>

#include <iostream>

namespace api
{
#if __cpp_concepts
    template<typename T>
    concept RequestHandlerConcept = requires(T & t)
    {
        t.can_handle();
        t.handle();
    };
#endif


    template<typename ResponseSerializer, typename... RequestHandlers>
#if __cpp_concepts
        requires (RequestHandlerConcept<RequestHandlers> && ...)
#endif
    struct request_dispatcher
    {

        request_dispatcher(ResponseSerializer && response_serializer, std::vector<std::variant<RequestHandlers...>> request_handlers) :
            response_serializer_{std::move(response_serializer)}, request_handlers_{std::move(request_handlers)}
        {
        }
        
        boost::beast::http::response<boost::beast::http::string_body>
        dispatch(const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            std::vector<std::string> target_split;
            boost::split(target_split, request.target(), boost::is_any_of("?"));
            boost::urls::decode_view dv {target_split.at(0)};
            std::cout << dv;
            std::string route {dv.begin(), dv.end()};
            if (route.starts_with("/"))
            {
                route = route.substr(1, route.size());
            }

            std::vector<std::string> route_split;
            boost::split(route_split, route, boost::is_any_of("/"));

            auto it = std::find_if(request_handlers_.begin(), request_handlers_.end(), [&route_split](std::variant<RequestHandlers...> & request_handler_variant){
                return std::visit(
                    [&route_split](auto & rh){
                        return rh.can_handle(route_split);
                    },
                    request_handler_variant
                );
            });

            if (it != request_handlers_.end())
            {
                auto res_api = std::visit(
                    [&route_split, &request](auto & rh){
                        return rh.handle(route_split, request);
                    },
                    *it
                );
                return std::visit(
                    [&request, &res_api, this](auto & res) {
                        return this->response_serializer_(request, res);
                    },
                    res_api
                );
            }
            else
            {
                return {boost::beast::http::status::not_found, request.version()};
            }
        }

    private:

        ResponseSerializer response_serializer_;
        std::vector<std::variant<RequestHandlers...>> request_handlers_;

    };
    
}