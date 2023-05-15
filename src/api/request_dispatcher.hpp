#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/beast/http.hpp>
#include <variant>

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


    template<typename... RequestHandlers>
#if __cpp_concepts
        requires (RequestHandlerConcept<RequestHandlers> && ...)
#endif
    struct request_dispatcher
    {

        request_dispatcher(std::vector<std::variant<RequestHandlers...>> request_handlers) :
            request_handlers_{std::move(request_handlers)}
        {
        }
        
        boost::beast::http::response<boost::beast::http::string_body>
        dispatch(const boost::beast::http::request<boost::beast::http::string_body> & request)
        {
            std::vector<std::string> target_split;
            boost::split(target_split, request.target(), boost::is_any_of("?"));
            std::string route = target_split.at(0);
            if (route.starts_with("/"))
            {
                route = route.substr(1, route.size());
            }

            std::vector<std::string> route_split;
            boost::split(route_split, route, boost::is_any_of("?"));

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
                return std::visit(
                    [&route_split, &request](auto & rh){
                        return rh.handle(route_split, request);
                    },
                    *it
                );
            }
            else
            {
                return {boost::beast::http::status::not_found, request.version()};
            }
        }

    private:

        std::vector<std::variant<RequestHandlers...>> request_handlers_;

    };
    
}