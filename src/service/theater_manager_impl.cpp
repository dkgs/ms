#include "theater_manager_impl.hpp"

#include <algorithm>

namespace service {

    theater_manager_impl::theater_manager_impl(std::string name, std::vector<std::shared_ptr<show_manager>> show_managers) :
        name_{std::move(name)}, show_managers_{std::move(show_managers)}
    {
    }

    const std::string &
    theater_manager_impl::get_name() const
    {
        return name_;
    }

    bool
    theater_manager_impl::add_show_manager(std::shared_ptr<show_manager> show_manager)
    {
        std::lock_guard<std::shared_mutex> lock {mutex_};
        auto it = std::find_if(
            show_managers_.begin(),
            show_managers_.end(),
            [&show_manager](const auto & sm) {
                return sm->get_movie() == show_manager->get_movie();
            }
        );
        if (it == show_managers_.end())
        {
            show_managers_.emplace_back(std::move(show_manager));
            return true;
        }
        else {
            return false;
        }
    }

    std::vector<model::movie>
    theater_manager_impl::fetch_movies() const
    {
        std::shared_lock<std::shared_mutex> lock {mutex_};
        std::vector<model::movie> movies {show_managers_.size()};
        std::transform(show_managers_.begin(), show_managers_.end(), movies.begin(), [](const std::shared_ptr<show_manager> & show_manager) {
            return show_manager->get_movie();
        });
        return movies;
    }

    std::optional<std::shared_ptr<show_manager>>
    theater_manager_impl::get_show_manager(const std::string & movie_name) const
    {
        std::shared_lock<std::shared_mutex> lock {mutex_};
        auto it = std::find_if(show_managers_.begin(), show_managers_.end(), [&movie_name](const std::shared_ptr<show_manager> & show_manager) {
            return show_manager->get_movie().name == movie_name;
        });
        if (it == show_managers_.end())
        {
            return {};
        }
        else
        {
            return {*it};
        }
    }

}
