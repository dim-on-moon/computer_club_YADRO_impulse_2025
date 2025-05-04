#include "Time.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Time::Time(const std::string &time_str)
{
    // Ожидаем формат ""HH:MM"
    if (time_str.size() != 5 || time_str[2] != ':')
    {
        throw std::invalid_argument("Invalid time format. Expected HH:MM");
    }

    // Парсим часы и минуты
    std::istringstream ss(time_str);
    char colon;
    ss >> hours_ >> colon >> minutes_;

    if (ss.fail() || !validate())
    {
        throw std::invalid_argument("Invalid time values. Hours: 0-23, Minutes: 0-59");
    }
}

bool Time::operator<(const Time &other) const
{
    if (hours_ != other.hours_)
    {
        return hours_ < other.hours_;
    }

    return minutes_ < other.minutes_;
}

bool Time::operator==(const Time &other) const
{
    return (minutes_ == other.minutes_) && (hours_ == other.hours_);
}

bool Time::operator<=(const Time &other) const
{
    return (*this < other) || (*this == other);
}

std::string Time::to_string() const
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours_ << ":"
       << std::setfill('0') << std::setw(2) << minutes_;
    return ss.str();
}

bool Time::validate() const
{
    return hours_ >= 0 && hours_ <= 23 && minutes_ >= 0 && minutes_ <= 59;
}