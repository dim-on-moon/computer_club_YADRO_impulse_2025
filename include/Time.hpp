#pragma once
#include <string>

class Time
{
public:
    // Дефолтный конструктор (00:00)
    Time() = default;

    // Конструктор из строки
    explicit Time(const std::string &time_str); // Парсит "HH:MM"

    // Операторы сравнения
    bool operator<(const Time &other) const;
    bool operator==(const Time &other) const;
    bool operator<=(const Time &other) const;

    // Возвращает время в формате "HH:MM"
    std::string to_string() const;

    // Геттеры
    int get_hours() const { return hours_; }
    int get_minutes() const { return minutes_; }

private:
    int hours_{0};
    int minutes_{0};

    // Проверяет корректность HH:MM
    bool validate() const; 
};