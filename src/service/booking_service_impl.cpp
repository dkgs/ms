#include "booking_service_impl.hpp"

#include <set>

namespace service 
{

    booking_service_impl::booking_service_impl(std::vector<std::shared_ptr<theater_manager>> theater_managers) :
        theater_managers_{std::move(theater_managers)}
    {
    }

    std::vector<model::movie>
    booking_service_impl::get_movies() const
    {
        auto movie_less = [](const model::movie & lhs, const model::movie & rhs)
        {
            return std::less<std::string>{}(lhs.name, rhs.name);
        };
        std::set<model::movie, decltype(movie_less)> movie_set {movie_less};

        for (const auto & theater_manager : theater_managers_)
        {
            for (const auto & movie : theater_manager->fetch_movies())
            {
                movie_set.emplace(movie);
            }
        }
        return {movie_set.begin(), movie_set.end()};
    }

    std::vector<std::string>
    booking_service_impl::fetch_theater_names_that_plays(const std::string & movie_name) const
    {
        auto movie_predicate = [&movie_name](const model::movie& m) { return movie_name == m.name; };
        std::vector<std::string> theater_names;
        for (const auto & theater : theater_managers_)
        {
            auto movies = theater->fetch_movies();
            if (std::find_if(movies.begin(), movies.end(), movie_predicate) != movies.end())
            {
                theater_names.emplace_back(theater->get_name());
            }
        }
        return theater_names;
    }

    std::optional<std::shared_ptr<theater_manager>>
    booking_service_impl::get_theater_manager(const std::string & theater_name) const
    {
        auto theater_predicate = [&theater_name](const std::shared_ptr<theater_manager> & tm) { return theater_name == tm->get_name(); };
        auto it = std::find_if(theater_managers_.begin(), theater_managers_.end(), theater_predicate);
        if (it == theater_managers_.end())
        {
            return {};
        }
        else
        {
            return {*it};
        }
    }

}
