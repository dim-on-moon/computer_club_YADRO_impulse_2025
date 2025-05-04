#pragma once
#include "ClubConfig.hpp"
#include "Client.hpp"
#include "Table.hpp"
#include "Events.hpp"
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <memory>
#include <iostream>

class ClubManager
{
public:
    // Конструктор: принимает имя входного файла
    explicit ClubManager(const std::string &input_file);

    // Обрабатывает входящие события и генерирует вывод
    void run();

private:
    std::string input_file_;                                 // Имя входного файла
    ClubConfig config_;                                      // Конфигурация клуба
    std::map<std::string, std::unique_ptr<Client>> clients_; // Имя клиента -> клиент
    std::vector<std::unique_ptr<Table>> tables_;             // Столы
    std::queue<std::string> waiting_queue_;                  // Имена клиентов в очереди
    std::vector<std::unique_ptr<IEvent>> events_;            // Все события

    // Читает входной файл и обрабатывает события
    // Завершает программу при ошибке парсинга, выводя проблемную строку
    void parse_input();

    // Инициализирует конфигурацию, завершает программу в случае некорректных входных данных
    void initialize_config(std::istream& file);

    // Обрабатывает одно событие
    void process_event(const IEvent &event);

    // Закрывает день
    void finalize_day();

    // Печатает отчёт
    void output_results() const;

    // Обработчики событий
    void handle_client_arrived(const ClientArrivedEvent &event);
    void handle_client_sat(const ClientSatEvent &event);
    void handle_client_waiting(const ClientWaitingEvent &event);
    void handle_client_left(const ClientLeftEvent &event);
    void handle_client_left_end_day(const ClientLeftEndOfDayEvent &event);

    // Проверка введенного имени клиента на допустимые символы
    bool validate_client_name(const std::string &name) const;

    // Возвращает номер свободного стола (если свободных столов нет - 0)
    size_t get_free_table() const;

    // Назначение стола из клиенту в очереди
    void assign_table_from_queue(const Time &time, size_t table_id);
    void remove_client_from_queue(const std::string& client_name);
};