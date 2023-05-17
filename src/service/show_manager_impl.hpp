#include "show_manager.hpp"

#include <shared_mutex>

namespace service
{

    /**
     * @brief Simple implementation that considers that all the seats are aligned in a single row
     * 
     */
    struct show_manager_impl : public show_manager
    {       

        show_manager_impl(const model::movie & movie, unsigned int seats_capacity = 20);

        const model::movie &
        get_movie() const override;

        std::vector<std::vector<model::seat>>
        fetch_available_seats(unsigned int number_of_contiguous_seats) const override;

        std::vector<unsigned int>
        authorized_seat_group_sizes() const override;

        bool
        book_seats(const std::vector<std::string> & seat_ids) override;

    private:

        model::movie movie_;
        std::vector<model::seat> seats_;

        std::vector<model::seat>
        generate_seats(unsigned int seats_capacity) const;

        mutable std::shared_mutex mutex_;

    };

}