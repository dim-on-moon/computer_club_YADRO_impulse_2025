#include "Events.hpp"
#include <stdexcept>
#include <sstream>

ClientArrivedEvent::ClientArrivedEvent(const Time &time, const std::string &client_name) : IEvent(time, 1),
                                                                                           client_name_(client_name) {}

std::string ClientArrivedEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << client_name_;
    return ss.str();
}

ClientSatEvent::ClientSatEvent(const Time &time, const std::string &client_name, size_t table_id) : IEvent(time, 2),
                                                                                                    client_name_(client_name),
                                                                                                    table_id_(table_id) {}

std::string ClientSatEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << client_name_ << " " << table_id_;
    return ss.str();
}

ClientWaitingEvent::ClientWaitingEvent(const Time &time, const std::string &client_name) : IEvent(time, 3),
                                                                                           client_name_(client_name) {}

std::string ClientWaitingEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << client_name_;
    return ss.str();
}

ClientLeftEvent::ClientLeftEvent(const Time &time, const std::string &client_name) : IEvent(time, 4),
                                                                                     client_name_(client_name) {}

std::string ClientLeftEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << client_name_;
    return ss.str();
}

ClientLeftEndOfDayEvent::ClientLeftEndOfDayEvent(const Time &time, const std::string &client_name) : IEvent(time, 11),
                                                                                                     client_name_(client_name) {}

std::string ClientLeftEndOfDayEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << client_name_;
    return ss.str();
}

ClientSatFromQueueEvent::ClientSatFromQueueEvent(const Time &time, const std::string &client_name, size_t table_id) : IEvent(time, 12),
                                                                                                                      client_name_(client_name),
                                                                                                                      table_id_(table_id) {}

std::string ClientSatFromQueueEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << client_name_ << " " << table_id_;
    return ss.str();
}

ErrorEvent::ErrorEvent(const Time &time, const std::string &message) : IEvent(time, 13),
                                                                       message_(message) {}

std::string ErrorEvent::to_string() const
{
    std::stringstream ss;
    ss << time_.to_string() << " " << id_ << " " << message_;
    return ss.str();
}