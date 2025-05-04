#pragma once
#include <string>

class Client
{
public:
    // Конструктор с именем клиента
    explicit Client(const std::string &name);

    // Геттеры
    std::string get_name() const { return name_; }
    bool is_in_club() const { return in_club_; }
    bool is_in_waiting() const { return in_waiting_; }
    bool is_at_table() const { return at_table_; }
    size_t get_table_id() const { return table_id_; }

    // Клиент входит в клуб
    void enter_club();

    // Клиент становится в очередь  
    void wait();

    // Клиент больше не в очереди
    void leave_waiting_list();

    // Клиент садится за стол
    void sit_at_table(size_t table_id);

    // Клиент покидает клуб
    void leave_club();

private:
    std::string name_;       // Имя клиента
    bool in_club_{false};    // В клубе ли клиент
    bool in_waiting_{false}; // В очереди ли клиент
    bool at_table_{false};   // За столом ли клиент
    size_t table_id_{0};     // Номер стола
};