#include "Table.hpp"
#include <stdexcept>
#include <iomanip>
#include <sstream>

Table::Table(size_t id) : id_(id) {}

void Table::occupy(const std::string &client_name, const Time &start_time)
{
    occupied_ = true;
    current_client_ = client_name;
    start_time_ = start_time;
}

void Table::free(const Time &end_time, int hourly_rate)
{
    update_stats(end_time, hourly_rate);
    occupied_ = false;
    current_client_.clear();
}

void Table::update_stats(const Time &end_time, int hourly_rate)
{
    // Считаем разницу времени в минутах
    int total_minutes = (end_time.get_hours() * 60 + end_time.get_minutes()) -
                        (start_time_.get_hours() * 60 + start_time_.get_minutes());

    // Обновляем общее время занятости
    int current_occupied_minutes = total_occupied_time_.get_hours() * 60 +
                                   total_occupied_time_.get_minutes();
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << (current_occupied_minutes + total_minutes) / 60 << ":"
       << std::setfill('0') << std::setw(2) << (current_occupied_minutes + total_minutes) % 60;
    total_occupied_time_ = Time(ss.str());

    // Считаем выручку: округляем вверх до полного часа
    int hours = (total_minutes + 59) / 60;
    revenue_ += hours * hourly_rate;
}