#include "api/request_dispatcher.hpp"
#include "api/connection_manager.hpp"
#include "api/handler/book_seat_handler.hpp"
#include "api/handler/choose_seat_handler.hpp"
#include "api/handler/choose_theater_handler.hpp"
#include "api/handler/root_handler.hpp"
#include "api/handler/health_handler.hpp"
#include "api/serializer/html_serializer.hpp"

#include "service/booking_service_impl.hpp"
#include "service/theater_manager_impl.hpp"
#include "service/show_manager_impl.hpp"

#include <iostream>
#include <memory>

/**
 * @brief Create a and populate booking service object.
 * Sets all the data in memory.
 * 
 * @return std::shared_ptr<service::booking_service_impl> 
 */
static
std::shared_ptr<service::booking_service_impl>
create_and_populate_booking_service()
{
    const service::model::movie the_godfather {
        .name = "The Godfather",
        .synopsis = "The aging patriarch of an organized crime dynasty transfers control of his clandestine empire to his reluctant son."
    };
    const service::model::movie pulp_fiction {
        .name = "Pulp Fiction",
        .synopsis = "The lives of two mob hitmen, a boxer, a gangster's wife, and a pair of diner bandits intertwine in four tales of violence and redemption."
    };
    const service::model::movie forrest_gump {
        .name = "Forrest Gump",
        .synopsis = "The story of Forrest Gump, a simple man with a low IQ who becomes a football star, war hero, and successful businessman, all while trying to win the heart of his childhood sweetheart."
    };
    const service::model::movie the_matrix {
        .name = "The Matrix",
        .synopsis = "A computer hacker learns from mysterious rebels about the true nature of his reality and his role in the war against its controllers."
    };
    return std::make_shared<service::booking_service_impl>(std::vector<std::shared_ptr<service::theater_manager>>{
        std::make_shared<service::theater_manager_impl>("Vox Marina Mall", std::vector<std::shared_ptr<service::show_manager>>{
            std::make_shared<service::show_manager_impl>(the_godfather),
            std::make_shared<service::show_manager_impl>(pulp_fiction),
            std::make_shared<service::show_manager_impl>(forrest_gump)
        }),
        std::make_shared<service::theater_manager_impl>("Novo Al Wahda Mall", std::vector<std::shared_ptr<service::show_manager>>{
            std::make_shared<service::show_manager_impl>(pulp_fiction),
            std::make_shared<service::show_manager_impl>(the_matrix)
        }),
        std::make_shared<service::theater_manager_impl>("Cinemax", std::vector<std::shared_ptr<service::show_manager>>{
            std::make_shared<service::show_manager_impl>(forrest_gump),
            std::make_shared<service::show_manager_impl>(the_matrix)
        })
    });
}

int main(int argc, char * argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr
                << "2 arguments are expected\n"
                << "Usage: ms <address> <port>"
                << std::endl;
            return -1;
        }

        auto booking_service = create_and_populate_booking_service();

        using request_dispatcher_t = api::request_dispatcher<
            api::serializer::html_serializer,
            api::handler::book_seat_handler,
            api::handler::choose_seat_handler,
            api::handler::choose_theater_handler,
            api::handler::root_handler,
            api::handler::health_handler
        >;
        using connection_manager_t = api::connection_manager<request_dispatcher_t>;

        request_dispatcher_t rd {
            api::serializer::html_serializer{},
            {
                api::handler::book_seat_handler{booking_service},
                api::handler::choose_seat_handler{booking_service},
                api::handler::choose_theater_handler{booking_service},
                api::handler::root_handler{booking_service},
                api::handler::health_handler{}
            }
        };
        connection_manager_t cm {std::move(rd), booking_service};
        cm.run(argv[1], static_cast<unsigned short>(std::atoi(argv[2])));

    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
