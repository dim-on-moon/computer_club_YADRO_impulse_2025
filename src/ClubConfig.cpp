#include "ClubConfig.hpp"
#include <stdexcept>

ClubConfig::ClubConfig(size_t num_tables, Time start_time, Time end_time, int hourly_rate) : num_tables_(num_tables),
                                                                                             start_time_(start_time),
                                                                                             end_time_(end_time),
                                                                                             hourly_rate_(hourly_rate) {}

bool ClubConfig::is_within_open_hours(const Time &time) const
{
    return start_time_ <= time && time <= end_time_;
}