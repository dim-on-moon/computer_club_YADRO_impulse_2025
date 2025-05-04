#include "ClubManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

ClubManager::ClubManager(const std::string &input_file) : input_file_(input_file),
                                                          config_(1, Time("00:00"), Time("23:59"), 1) {}
// Временная инициализация config_, будет перезаписан в initialize_config()

void ClubManager::run()
{
    parse_input();
    finalize_day();
    output_results();
}

void ClubManager::parse_input()
{
    // Открываем входной файл и передаем в initialize_config()
    std::ifstream file(input_file_);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << input_file_ << std::endl;
        std::exit(1);
    }
    initialize_config(file);

    // Инициализируем столы
    tables_.clear();
    for (size_t i = 1; i <= config_.get_num_tables(); ++i)
    {
        tables_.push_back(std::make_unique<Table>(i));
    }

    // Читаем и обрабатываем события
    std::string event_line;
    while (std::getline(file, event_line))
    {
        try
        {
            std::stringstream event_ss(event_line);
            std::string time_str, client_name;
            int id;
            size_t table_id;

            event_ss >> time_str >> id;
            Time time(time_str);

            std::unique_ptr<IEvent> event;
            switch (id)
            {
            case 1: // Клиент пришёл
                event_ss >> client_name;
                if (!validate_client_name(client_name))
                {
                    throw std::invalid_argument("Invalid client's name");
                }
                event = std::make_unique<ClientArrivedEvent>(time, client_name);
                break;
            case 2: // Клиент сел за стол
                event_ss >> client_name >> table_id;
                if (!validate_client_name(client_name))
                {
                    throw std::invalid_argument("Invalid client's name");
                }
                if (table_id == 0 || table_id > config_.get_num_tables())
                {
                    throw std::invalid_argument("Incorrect table's ID = 0");
                }
                event = std::make_unique<ClientSatEvent>(time, client_name, table_id);
                break;
            case 3: // Клиент ждёт
                event_ss >> client_name;
                if (!validate_client_name(client_name))
                {
                    throw std::invalid_argument("Invalid client's name");
                }
                event = std::make_unique<ClientWaitingEvent>(time, client_name);
                break;
            case 4: // Клиент ушёл
                event_ss >> client_name;
                if (!validate_client_name(client_name))
                {
                    throw std::invalid_argument("Invalid client's name");
                }
                event = std::make_unique<ClientLeftEvent>(time, client_name);
                break;
            default:
                throw std::invalid_argument("Invalid event ID");
            }

            // Добавляем входящее событие в вектор событий и обрабатываем его дальше в методе process_event()
            events_.push_back(std::move(event));
            process_event(*events_.back());
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << event_line << std::endl;
            std::exit(1);
        }
    }
}

void ClubManager::initialize_config(std::istream &file)
{
    std::string num_tables_line, times_line, hourly_rate_line;

    // Читаем число столов
    if (!std::getline(file, num_tables_line))
    {
        std::cout << "Empty file" << std::endl;
        std::exit(1);
    }
    size_t num_tables;
    try
    {
        num_tables = std::stoul(num_tables_line);
        if (num_tables == 0)
        {
            throw std::invalid_argument("Number of tables is equal to zero");
        }
    }
    catch (...)
    {
        std::cout << num_tables_line << std::endl;
        std::exit(1);
    }

    // Читаем время работы
    if (!std::getline(file, times_line))
    {
        std::cout << "Missing time configuration line" << std::endl;
        std::exit(1);
    }
    std::stringstream times_ss(times_line);
    std::string open_time_str, close_time_str;
    Time open_time, close_time;
    try
    {
        times_ss >> open_time_str >> close_time_str;
        if (open_time_str.empty() || close_time_str.empty())
        {
            std::cout << times_line << std::endl;
            std::exit(1);
        }
        open_time = Time(open_time_str);   // Может выбросить std::invalid_argument
        close_time = Time(close_time_str); // Может выбросить std::invalid_argument
        if (close_time <= open_time)
        {
            throw std::invalid_argument("The opening time cannot be later than the closing time");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << times_line << std::endl;
        std::exit(1);
    }

    // Читаем стоимость часа
    if (!std::getline(file, hourly_rate_line))
    {
        std::cout << "Missing hourly rate line" << std::endl;
        std::exit(1);
    }
    int hourly_rate;
    try
    {
        hourly_rate = std::stoi(hourly_rate_line);
        if (hourly_rate <= 0)
        {
            throw std::invalid_argument("The hourly rate must be positive");
        }
    }
    catch (...)
    {
        std::cout << hourly_rate_line << std::endl;
        std::exit(1);
    }

    // Все параметры удовлетворяют требованиям, задаем конфигурацию клуба
    config_ = ClubConfig(num_tables, open_time, close_time, hourly_rate);
}

void ClubManager::process_event(const IEvent &event)
{
    switch (event.get_id())
    {
    case 1:
        handle_client_arrived(dynamic_cast<const ClientArrivedEvent &>(event));
        break;
    case 2:
        handle_client_sat(dynamic_cast<const ClientSatEvent &>(event));
        break;
    case 3:
        handle_client_waiting(dynamic_cast<const ClientWaitingEvent &>(event));
        break;
    case 4:
        handle_client_left(dynamic_cast<const ClientLeftEvent &>(event));
        break;
    default:
        break;
    }
}

void ClubManager::finalize_day()
{
    Time close_time = config_.get_end_time();

    // Получаем и сортируем по алфавиту список имен клиентов, которые еще в клубе
    std::vector<std::string> client_names;
    for (const auto &pair : clients_)
    {
        client_names.push_back(pair.first);
    }
    std::sort(client_names.begin(), client_names.end());

    // Отпускаем клиентов
    for (const auto &name : client_names)
    {
        std::unique_ptr<IEvent> event = std::make_unique<ClientLeftEndOfDayEvent>(close_time, name);
        events_.push_back(std::move(event));
        handle_client_left_end_day(dynamic_cast<const ClientLeftEndOfDayEvent &>(*events_.back()));
    }

    clients_.clear();
    waiting_queue_ = std::queue<std::string>();
}

void ClubManager::output_results() const
{
    // Выводим время начала работы
    std::cout << config_.get_start_time().to_string() << std::endl;

    // Выводим все события (входящие, исходящие, ошибки)
    for (const auto &event : events_)
    {
        std::cout << event->to_string() << std::endl;
    }

    // Выводим время окончания работы
    std::cout << config_.get_end_time().to_string() << std::endl;

    // Выводим статистику столов
    for (const auto &table : tables_)
    {
        std::cout << table->get_id() << " " << table->get_revenue() << " "
                  << table->get_total_occupied_time().to_string() << std::endl;
    }
}

void ClubManager::handle_client_arrived(const ClientArrivedEvent &event)
{
    const std::string &name = event.get_client_name();
    const Time &time = event.get_time();

    // Проверка, что клиент еще не в клубе
    if (clients_.find(name) != clients_.end())
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "YouShallNotPass"));
        return;
    }

    // Проверка рабочих часов
    if (!config_.is_within_open_hours(time))
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "NotOpenYet"));
        return;
    }

    // Добавляем клиента
    clients_[name] = std::make_unique<Client>(name);
    clients_[name]->enter_club();
}

void ClubManager::handle_client_sat(const ClientSatEvent &event)
{
    const std::string &name = event.get_client_name();
    const Time &time = event.get_time();
    size_t table_id = event.get_table_id();

    // Проверка, что клиент в клубе
    auto it = clients_.find(name);
    if (it == clients_.end())
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "ClientUnknown"));
        return;
    }

    // Проверка, что стол не занят
    if (tables_[table_id - 1]->is_occupied())
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "PlaceIsBusy"));
        return;
    }

    // Если клиент уже за столом, освобождаем старый стол
    if (it->second->is_at_table())
    {
        size_t old_table_id = it->second->get_table_id();
        tables_[old_table_id - 1]->free(time, config_.get_hourly_rate());
    }

    // Клиент садится за стол
    it->second->sit_at_table(table_id);
    tables_[table_id - 1]->occupy(name, time);

    // Если клиент был в очереди - он больше не в очереди
    remove_client_from_queue(name);
}

void ClubManager::handle_client_waiting(const ClientWaitingEvent &event)
{
    const std::string &name = event.get_client_name();
    const Time &time = event.get_time();

    // Проверка, что клиент в клубе
    auto it = clients_.find(name);
    if (it == clients_.end())
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "ClientUnknown"));
        return;
    }

    // Проверка, что нет свободных столов
    if (get_free_table() != 0)
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "ICanWaitNoLonger!"));
        return;
    }

    // Проверка размера очереди
    if (waiting_queue_.size() >= tables_.size())
    {
        events_.push_back(std::make_unique<ClientLeftEndOfDayEvent>(time, name));
        it->second->leave_club();
        clients_.erase(it);
        return;
    }

    // Клиент становится в очередь
    it->second->wait();
    waiting_queue_.push(name);
}

void ClubManager::handle_client_left(const ClientLeftEvent &event)
{
    const std::string &name = event.get_client_name();
    const Time &time = event.get_time();

    // Проверка, что клиент в клубе
    auto it = clients_.find(name);
    if (it == clients_.end())
    {
        events_.push_back(std::make_unique<ErrorEvent>(time, "ClientUnknown"));
        return;
    }

    // Если клиент за столом, освобождаем стол
    if (it->second->is_at_table())
    {
        size_t table_id = it->second->get_table_id();
        tables_[table_id - 1]->free(time, config_.get_hourly_rate());
        assign_table_from_queue(time, table_id);
    }

    // Клиент уходит
    it->second->leave_club();
    clients_.erase(it);

    // Если клиент был в очереди - он больше не в очереди
    remove_client_from_queue(name);
}

void ClubManager::handle_client_left_end_day(const ClientLeftEndOfDayEvent &event)
{
    auto it = clients_.find(event.get_client_name());
    if (it->second->is_at_table())
    {
        size_t table_id = it->second->get_table_id();
        tables_[table_id - 1]->free(event.get_time(), config_.get_hourly_rate());
    }
    it->second->leave_club();
}

bool ClubManager::validate_client_name(const std::string &name) const
{
    if (name.empty())
    {
        return false;
    }

    for (char c : name)
    {
        if (!(std::isalnum(c) || c == '-' || c == '_'))
        {
            return false;
        }
    }
    return true;
}

size_t ClubManager::get_free_table() const
{
    for (size_t i = 0; i < tables_.size(); ++i)
    {
        if (!tables_[i]->is_occupied())
        {
            return i + 1;
        }
    }
    return 0; // Нет свободных столов
}

void ClubManager::assign_table_from_queue(const Time &time, size_t table_id)
{
    if (waiting_queue_.empty())
    {
        return;
    }

    // Первый клиент из очереди садится за стол
    std::string name = waiting_queue_.front();
    waiting_queue_.pop();

    auto it = clients_.find(name);
    if (it != clients_.end())
    {
        it->second->sit_at_table(table_id);
        tables_[table_id - 1]->occupy(name, time);
        events_.push_back(std::make_unique<ClientSatFromQueueEvent>(time, name, table_id));
    }
}

void ClubManager::remove_client_from_queue(const std::string &client_name)
{
    std::queue<std::string> temp_queue;
    while (!waiting_queue_.empty())
    {
        std::string current = waiting_queue_.front();
        waiting_queue_.pop();
        if (current != client_name)
        {
            temp_queue.push(current);
        }
    }
    waiting_queue_ = temp_queue;
}