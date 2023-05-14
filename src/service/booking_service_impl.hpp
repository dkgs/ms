#include "booking_service.hpp"

namespace service 
{

    struct booking_service_impl : public booking_service
    {

        booking_service_impl(std::vector<std::shared_ptr<theater_manager>> theater_managers);
        ~booking_service_impl() = default;

        std::vector<model::movie>
        get_movies() const override;

        std::vector<std::string>
        fetch_theater_names_that_plays(const std::string & movie_name) const override;

        std::optional<std::shared_ptr<theater_manager>>
        get_theater_manager(const std::string & theater_name) const override;

    private:

        std::vector<std::shared_ptr<theater_manager>> theater_managers_;

    };

}
