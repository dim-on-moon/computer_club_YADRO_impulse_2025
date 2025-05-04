#pragma once
#include "Time.hpp"

class Table
{
public:
    // Конструктор с ID стола
    explicit Table(size_t id);

    // Проверяет, занят ли стол
    bool is_occupied() const { return occupied_; }

    // Клиент садится за стол
    void occupy(const std::string &client_name, const Time &start_time);

    // Клиент покидает стол
    void free(const Time &end_time, int hourly_rate);

    // Геттеры
    size_t get_id() const { return id_; }
    int get_revenue() const { return revenue_; }
    Time get_total_occupied_time() const { return total_occupied_time_; }

private:
    size_t id_;                  // Номер стола
    bool occupied_{false};       // Занят ли стол
    Time start_time_;            // Время, когда сел текущий клиент
    std::string current_client_; // Имя текущего клиента
    int revenue_{0};             // Выручка со стола
    Time total_occupied_time_;   // Сколько времени стол был занят

    // Обновляет статистику (выручку и время)
    void update_stats(const Time &end, int hourly_rate);
};