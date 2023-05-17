#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <string>

namespace api
{
#if __cpp_concepts
    /**
     * @brief RequestDispatcher concept that must be followed in order to be used in the connection manager.
     * 
     * @tparam T class to test against the concept.
     */
    template<typename T>
    concept RequestDispatcherConcept = requires(T t, const boost::beast::http::request<boost::beast::http::string_body> & request)
    {
        /**
         * @brief Shall be able to handle the dispatchment of a request
         */
        t.dispatch(request);
    };
#endif

    /**
     * @brief Handles the incoming connection and forward them to the request dispatcher.
     * 
     * @tparam RequestDispatcher type of the request handler to use, must respect the request handler concept.
     */
    template<typename RequestDispatcher>
#if __cpp_concepts
        requires RequestDispatcherConcept<RequestDispatcher>
#endif
    struct connection_manager
    {
        /**
         * @brief Construct a new connection manager object
         * 
         * @param request_dispatcher dispatches incoming http request
         */
        connection_manager(RequestDispatcher && request_dispatcher) :
            request_dispatcher_{std::move(request_dispatcher)}
        {}

        /**
         * @brief Runs the server on the specific address and port
         * 
         * @param str_address address of the server
         * @param port  port to listen
         */
        void
        run(const std::string & str_address, unsigned short port)
        {
            if (port <= 1024)
            {
                throw std::invalid_argument("The port must be higher than 1024");
            }
            const auto address = boost::asio::ip::make_address(str_address);

            // 4 could be replaced by hardware_concurrency()
            boost::asio::thread_pool pool{4};

            boost::asio::io_context context {};
            boost::asio::ip::tcp::acceptor acceptor {context, {address, port}};

            for(;;)
            {
                boost::asio::ip::tcp::socket socket{context};
                acceptor.accept(socket);

                boost::asio::post(pool, [socket = std::move(socket), this]() mutable { this->handle_request(std::move(socket)); });
            }

        }

    private:

        RequestDispatcher request_dispatcher_;

        void
        handle_request(boost::asio::ip::tcp::socket && socket)
        {
            try
            {
                bool close = false;
                boost::beast::flat_buffer buffer;

                boost::beast::http::request<boost::beast::http::string_body> req;
                boost::beast::http::read(socket, buffer, req);

                auto response = request_dispatcher_.dispatch(req);

                boost::beast::http::write(socket, response);

                socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
            }
            catch (const std::exception & e)
            {
                // TODO handle exception
            }
        }

    };

}