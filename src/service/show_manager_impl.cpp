#include "show_manager_impl.hpp"

#include <algorithm>

namespace service
{

    show_manager_impl::show_manager_impl(const model::movie & movie, unsigned int seats_capacity) :
        movie_{movie}, seats_{generate_seats(seats_capacity)}
    {}

    const model::movie &
    show_manager_impl::get_movie() const
    {
        return movie_;
    }

    std::vector<std::vector<model::seat>>
    show_manager_impl::fetch_available_seats(unsigned int number_of_contiguous_seats) const
    {
        std::shared_lock<std::shared_mutex> lock {mutex_};
        if (number_of_contiguous_seats == 0 || number_of_contiguous_seats > seats_.size())
        {
            return {};
        }
        std::vector<std::vector<model::seat>> available_seats {};
        // dumb implementation, can be optimized if there is a performance problem
        // check for each index that the number_of_contiguous_seats exists
        for (auto it = seats_.begin(); it < seats_.end() - number_of_contiguous_seats + 1; it++)
        {
            bool valid_flag = true;
            for (auto it_check = it; it_check < it + number_of_contiguous_seats; it_check++)
            {
                if (it_check->status == model::seat::status::RESERVED)
                {
                    valid_flag = false;
                    break;
                }
            }
            if (valid_flag)
            {
                available_seats.emplace_back(it, it+number_of_contiguous_seats);
            }
        }
        return available_seats;
    }

    std::vector<int>
    show_manager_impl::authorized_seat_group_sizes() const
    {
        return {1, 2, 3, 4};
    }

    bool
    show_manager_impl::book_seats(const std::vector<std::string> & seat_ids)
    {
        std::lock_guard<std::shared_mutex> lock{mutex_};
        std::vector<model::seat*> keeper;
        bool valid_flag = true;
        for (const auto & seat_id : seat_ids)
        {
            auto it = std::find_if(seats_.begin(), seats_.end(), [&seat_id](const model::seat & s){
                return s.id == seat_id;
            });
            if (it == seats_.end() || it->status == model::seat::status::RESERVED)
            {
                valid_flag = false;
                break;
            }
            keeper.push_back(&*it);
        }
        if (valid_flag) {
            for (auto seat_ptr : keeper)
            {
                seat_ptr->status = model::seat::status::RESERVED;
            }
        }
        return valid_flag;
    }

    std::vector<model::seat>
    show_manager_impl::generate_seats(unsigned int seats_capacity) const
    {
        std::vector<model::seat> seats {seats_capacity};
        for (unsigned int i = 0; i < seats_capacity; i++)
        {
            seats[i] = {
                .id = "a" + std::to_string(i+1),
                .status = model::seat::status::AVAILABLE
            };
        }
        return seats;
    }

}
