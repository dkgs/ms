#include "service/show_manager_impl.hpp"

#include <boost/asio.hpp>
#include <catch2/catch.hpp>

using namespace service;

SCENARIO("classic usage of the show manager")
{
    GIVEN("a show manager of a movie")
    {
        show_manager_impl show_manager {{.name="movie", .synopsis=""}, 5};

        WHEN("we request the movie")
        {
            auto movie = show_manager.get_movie();
            THEN("we get it")
            {
                REQUIRE( movie.name == "movie" );
            }
        }

        WHEN("we check individual available seats, when nothing has yet been booked")
        {
            auto seat_groups = show_manager.fetch_available_seats(1);
            THEN("we have 5 groups of 1 seat")
            {
                REQUIRE( seat_groups.size() == 5 );
                for (const auto seat_group : seat_groups)
                {
                    REQUIRE( seat_group.size() == 1 );
                }
            }
        }

        WHEN("we fetch available seats for a group of 3 when nothing has yet been booked")
        {
            auto seat_groups = show_manager.fetch_available_seats(3);
            THEN("we have 3 groups of 3 seats each")
            {
                REQUIRE( seat_groups.size() == 3 );
                for (const auto seat_group : seat_groups)
                {
                    REQUIRE( seat_group.size() == 3 );
                }
            }
        }

        WHEN("we fetch available seats for a group number that is not supported")
        {
            unsigned int big_number = 999;
            auto sizes = show_manager.authorized_seat_group_sizes();
            REQUIRE( std::find(sizes.begin(), sizes.end(), big_number) == sizes.end() );
            THEN("Must get an empty vector of available seats")
            {
                auto seat_groups = show_manager.fetch_available_seats(999);
                REQUIRE( seat_groups.empty() );
            }
        }

        WHEN("we book three seats right in the middle")
        {
            auto seat_groups = show_manager.fetch_available_seats(3);
            std::vector<std::string> seat_group_to_book {seat_groups.at(1).size()};
            std::transform(
                seat_groups.at(1).begin(), seat_groups.at(1).end(),
                seat_group_to_book.begin(), [](const model::seat & seat) { return seat.id; }
            );
            REQUIRE( show_manager.book_seats(seat_group_to_book) );
            THEN("there is on remaining seat on each side")
            {
                auto remaining_seats = show_manager.fetch_available_seats(1);
                REQUIRE( remaining_seats.size() == 2 );
                REQUIRE( remaining_seats.at(0).size() == 1 );
                REQUIRE( remaining_seats.at(0).at(0).id == "a1" );
                REQUIRE( remaining_seats.at(1).size() == 1 );
                REQUIRE( remaining_seats.at(1).at(0).id == "a5" );
            }
            THEN("there is no groups of 2 or more available")
            {
                REQUIRE( show_manager.fetch_available_seats(2).size() == 0 );
                REQUIRE( show_manager.fetch_available_seats(3).size() == 0 );
                REQUIRE( show_manager.fetch_available_seats(4).size() == 0 );
                REQUIRE( show_manager.fetch_available_seats(5).size() == 0 );
            }
        }
        WHEN("we fetch a larger contiguous than available")
        {
            auto seat_groups = show_manager.fetch_available_seats(6);
            THEN("there is no groups available")
            {
                REQUIRE( seat_groups.size() == 0 );
            }
        }
        WHEN("we fetch a zero sized group")
        {
            auto seat_groups = show_manager.fetch_available_seats(0);
            THEN("there is no groups available")
            {
                REQUIRE( seat_groups.size() == 0 );
            }
        }
    }

}

SCENARIO("a multi-threaded usage of the show manager")
{
    GIVEN("a show manager of a movie")
    {
        show_manager_impl show_manager {{.name="movie", .synopsis=""}, 5};

        WHEN("a bunch of client wants to book and in each booking there is at least the seat s3")
        {
            std::array<std::vector<std::string>, 10> seats_to_book {
                std::vector<std::string>{"a1", "a3"},
                std::vector<std::string>{"a2", "a3"},
                std::vector<std::string>{"a4", "a3"},
                std::vector<std::string>{"a5", "a3"},
                std::vector<std::string>{"a2", "a3"},
                std::vector<std::string>{"a1", "a2", "a3"},
                std::vector<std::string>{"a3", "a4"},
                std::vector<std::string>{"a1", "a3", "a4", "a5"},
                std::vector<std::string>{"a3", "a5"},
                std::vector<std::string>{"a1", "a3"}
            };
            std::array<bool, 10> results;
            boost::asio::thread_pool pool(4);
            for (unsigned int i = 0; i < 10; i++)
            {
                boost::asio::post(pool,
                    [&show_manager, &seats_to_book, &results, i]() {
                        results[i] = show_manager.book_seats(seats_to_book[i]);
                    }
                );
            }
            pool.join();

            THEN("only one gets the reservation")
            {
                REQUIRE( std::count(results.begin(), results.end(), true) == 1 );
            }
        }
    }

}