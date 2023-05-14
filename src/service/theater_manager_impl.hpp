#include "theater_manager.hpp"

#include <shared_mutex>

namespace service {

    /**
     * @brief A simple implentation that manage a theatre.
     * It also permits to add shows on the fly.
     * 
     */
    struct theater_manager_impl : public theater_manager
    {

        theater_manager_impl(std::string name, std::vector<std::shared_ptr<show_manager>> show_managers = {});

        const std::string &
        get_name() const override;

        /**
         * @brief Add a show to this theatre
         * 
         * @param show_manager the show manager to add
         * @return true if successfully added
         * @return false if show with the same name exists
         */
        bool
        add_show_manager(std::shared_ptr<show_manager> show_manager);

        std::vector<model::movie>
        fetch_movies() const override;

        std::optional<std::shared_ptr<show_manager>>
        get_show_manager(const std::string & movie_name) const override;

    private:

        std::string name_;
        std::vector<std::shared_ptr<show_manager>> show_managers_;

        mutable std::shared_mutex mutex_;

    };

}
