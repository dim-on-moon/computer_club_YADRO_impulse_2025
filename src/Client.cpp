#include "Client.hpp"
#include <stdexcept>

Client::Client(const std::string &name) : name_(name) {}

void Client::enter_club()
{
    in_club_ = true;
}

void Client::wait()
{
    in_waiting_ = true;
}

void Client::leave_waiting_list()
{
    in_waiting_ = false;
}

void Client::sit_at_table(size_t table_id)
{
    at_table_ = true;
    table_id_ = table_id;
}

void Client::leave_club()
{
    in_club_ = false;
    in_waiting_ = false;
    at_table_ = false;
    table_id_ = 0;
}