#pragma once
#include "Time.hpp"

class ClubConfig
{
public:
    // Конструктор с параметрами
    ClubConfig(size_t num_tables, Time start_time, Time end_time, int hourly_rate);

    // Геттеры
    size_t get_num_tables() const { return num_tables_; }
    Time get_start_time() const { return start_time_; }
    Time get_end_time() const { return end_time_; }
    int get_hourly_rate() const { return hourly_rate_; }

    // Проверяет, попадает ли переданное время в часы работы клуба
    bool is_within_open_hours(const Time &time) const;

private:
    size_t num_tables_; // Количество столов в клубе
    Time start_time_;   // Время открытия клуба
    Time end_time_;     // Время закрытия клуба
    int hourly_rate_;   // Почасовая ставка
};