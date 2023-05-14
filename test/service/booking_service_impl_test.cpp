#include "service/booking_service_impl.hpp"

#include <catch2/catch.hpp>

using namespace service;

struct theater_manager_mock : public theater_manager
{
    std::string name_;
    std::vector<service::model::movie> movies_;

    theater_manager_mock(std::string name, std::vector<service::model::movie> movies) 
        : name_{std::move(name)}, movies_{std::move(movies)}
    {}

    virtual
    const std::string &
    get_name() const  override
    {
        return name_;
    }

    bool
    add_show_manager(std::shared_ptr<show_manager>)
    {
        return false;
    }

    virtual
    std::vector<model::movie>
    fetch_movies() const override
    {
        return movies_;
    }

    std::optional<std::shared_ptr<show_manager>>
    get_show_manager(const std::string & movie_name) const override
    {
        return {};
    }
};

SCENARIO("classic usage of the booking service")
{
    GIVEN("A booking service with three teathers")
    {
        const model::movie movie1 {.name = "movie1", .synopsis = ""};
        const model::movie movie2 {.name = "movie2", .synopsis = ""};
        const model::movie movie3 {.name = "movie3", .synopsis = ""};
        const model::movie movie4 {.name = "movie4", .synopsis = ""};
        const model::movie movie_unknown {.name = "unknown", .synopsis = ""};
        
        booking_service_impl bs {
            {
                std::make_shared<theater_manager_mock>("Theater1", std::vector<service::model::movie>{movie1, movie2, movie3}),
                std::make_shared<theater_manager_mock>("Theater2", std::vector<service::model::movie>{movie2, movie3}),
                std::make_shared<theater_manager_mock>("Theater3", std::vector<service::model::movie>{movie3, movie4})
            }
        };

        WHEN("we fetch all the movies")
        {
            auto all_movies = bs.get_movies();
            THEN("they must all appear only once")
            {
                REQUIRE( all_movies.size() == 4 );
                REQUIRE( std::find(all_movies.begin(), all_movies.end(), movie1) != all_movies.end() );
                REQUIRE( std::find(all_movies.begin(), all_movies.end(), movie2) != all_movies.end() );
                REQUIRE( std::find(all_movies.begin(), all_movies.end(), movie3) != all_movies.end() );
                REQUIRE( std::find(all_movies.begin(), all_movies.end(), movie4) != all_movies.end() );
            }
        }

        WHEN("we fetch the theaters that plays movie1")
        {
            auto theaters = bs.fetch_theater_names_that_plays(movie1.name);
            THEN("we must have only the first theater")
            {
                REQUIRE( theaters.size() == 1 );
                REQUIRE( theaters.at(0) == "Theater1" );
            }
        }

        WHEN("we fetch the theaters that plays movie2")
        {
            auto theaters = bs.fetch_theater_names_that_plays(movie2.name);
            THEN("we must have the first and the second theater")
            {
                REQUIRE( theaters.size() == 2 );
                REQUIRE( std::find(theaters.begin(), theaters.end(), "Theater1") != theaters.end() );
                REQUIRE( std::find(theaters.begin(), theaters.end(), "Theater2") != theaters.end() );
            }
        }

        WHEN("we fetch the theaters that plays movie3")
        {
            auto theaters = bs.fetch_theater_names_that_plays(movie3.name);
            THEN("we must have all the theaters")
            {
                REQUIRE( theaters.size() == 3 );
                REQUIRE( std::find(theaters.begin(), theaters.end(), "Theater1") != theaters.end() );
                REQUIRE( std::find(theaters.begin(), theaters.end(), "Theater2") != theaters.end() );
                REQUIRE( std::find(theaters.begin(), theaters.end(), "Theater3") != theaters.end() );
            }
        }

        WHEN("we fetch the theaters that plays movie4")
        {
            auto theaters = bs.fetch_theater_names_that_plays(movie4.name);
            THEN("we must have only the third theater")
            {
                REQUIRE( theaters.size() == 1 );
                REQUIRE( theaters.at(0) == "Theater3" );
            }
        }

        WHEN("we fetch the theaters with a movie that does not exists")
        {
            auto theaters = bs.fetch_theater_names_that_plays(movie_unknown.name);
            THEN("we must have no theater")
            {
                REQUIRE( theaters.size() == 0 );
            }
        }

        WHEN("we fetch a theater by its name")
        {
            auto tm = bs.get_theater_manager("Theater1");
            THEN("we must have the said theater")
            {
                REQUIRE( tm.has_value() );
                REQUIRE( tm.value()->get_name() == "Theater1" );
            }
        }

        WHEN("we fetch an unknown theater by its name")
        {
            auto tm = bs.get_theater_manager("unknown");
            THEN("we must have nothing")
            {
                REQUIRE( !tm.has_value() );
            }
        }
    }

    GIVEN("A booking service with no theaters")
    {
        booking_service_impl bs { {} };

        WHEN("we fetch the movies")
        {
            auto movies = bs.get_movies();
            THEN("there must be none")
            {
                REQUIRE( movies.size() == 0 );
            }
        }

        WHEN("we fetch a theather by a movie")
        {
            auto tm = bs.fetch_theater_names_that_plays("whatever");
            THEN("there must be none")
            {
                REQUIRE( tm.size() == 0 );
            }
        }

        WHEN("we fetch a theater manager")
        {
            auto tm = bs.get_theater_manager("whatever");
            THEN("there must be none")
            {
                REQUIRE( !tm.has_value() );
            }
        }
    }

}