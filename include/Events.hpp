#pragma once
#include <string>
#include "Time.hpp"

class IEvent
{
public:
    // Виртуальный деструктор для полиоморфизма
    virtual ~IEvent() = default;

    // Геттеры
    virtual Time get_time() const = 0;
    virtual int get_id() const = 0;

    // Преобразование события в строку
    virtual std::string to_string() const = 0;

protected:
    IEvent(const Time &time, int id) : time_(time), id_(id) {}
    Time time_;
    int id_;
};

// Событие: клиент пришёл (ID 1, входящее)
class ClientArrivedEvent : public IEvent
{
public:
    ClientArrivedEvent(const Time &time, const std::string &client_name);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    std::string get_client_name() const { return client_name_; }
    std::string to_string() const override;

private:
    std::string client_name_;
};

// Событие: клиент сел за стол (ID 2, входящее)
class ClientSatEvent : public IEvent
{
public:
    ClientSatEvent(const Time &time, const std::string &client_name, size_t table_id);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    size_t get_table_id() const { return table_id_; }
    std::string get_client_name() const { return client_name_; }
    std::string to_string() const override;

private:
    std::string client_name_;
    size_t table_id_;
};

// Событие: клиент ждёт в очереди (ID 3, входящее)
class ClientWaitingEvent : public IEvent
{
public:
    ClientWaitingEvent(const Time &time, const std::string &client_name);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    std::string get_client_name() const { return client_name_; }
    std::string to_string() const override;

private:
    std::string client_name_;
};

// Событие: клиент ушёл (ID 4, входящее)
class ClientLeftEvent : public IEvent
{
public:
    ClientLeftEvent(const Time &time, const std::string &client_name);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    std::string get_client_name() const { return client_name_; }
    std::string to_string() const override;

private:
    std::string client_name_;
};

// Событие: клиент ушёл в коцне дня (ID 11, исходящее)
class ClientLeftEndOfDayEvent : public IEvent
{
public:
    ClientLeftEndOfDayEvent(const Time &time, const std::string &client_name);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    std::string get_client_name() const { return client_name_; }
    std::string to_string() const override;

private:
    std::string client_name_;
};

// Событие: клиент из очереди сел за стол (ID 12, исходящее)
class ClientSatFromQueueEvent : public IEvent
{
public:
    ClientSatFromQueueEvent(const Time &time, const std::string &client_name, size_t table_id);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    size_t get_table_id() const { return table_id_; }
    std::string get_client_name() const { return client_name_; }
    std::string to_string() const override;

private:
    std::string client_name_;
    size_t table_id_;
};

// Событие ошибки (ID 13)
class ErrorEvent : public IEvent
{
public:
    ErrorEvent(const Time &time, const std::string &message);
    Time get_time() const override { return time_; }
    int get_id() const override { return id_; }
    std::string get_message() const { return message_; }
    std::string to_string() const override;

private:
    std::string message_;
};