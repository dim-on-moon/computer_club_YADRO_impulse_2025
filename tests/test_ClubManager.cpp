#include <gtest/gtest.h>
#include "ClubManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

class ClubManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Создаём временный файл с корректными данными
        std::ofstream file("test_input.txt");
        file << "2\n"
             << "09:00 23:00\n"
             << "100\n"
             << "10:00 1 alice\n"
             << "10:05 2 alice 1\n"
             << "10:10 1 bob\n"
             << "10:15 3 bob\n"
             << "10:20 2 bob 2\n"
             << "10:30 4 alice\n"
             << "10:35 4 bob\n";
        file.close();
    }

    void TearDown() override
    {
        std::remove("test_input.txt");
        std::remove("invalid_num_tables.txt");
        std::remove("invalid_time.txt");
        std::remove("invalid_hourly_rate.txt");
        std::remove("invalid_event_time.txt");
        std::remove("invalid_client_name.txt");
        std::remove("invalid_event_id.txt");
        std::remove("invalid_table_id.txt");
        std::remove("empty_file.txt");
        std::remove("cout_output.txt");
    }
};

// Тест нормальной работы
TEST_F(ClubManagerTest, ProcessEvents)
{
    std::stringstream cout_buffer;
    std::streambuf *cout_old = std::cout.rdbuf(cout_buffer.rdbuf());
    ClubManager manager("test_input.txt");
    manager.run();
    std::cout.rdbuf(cout_old);
    std::string output = cout_buffer.str();

    std::string expected =
        "09:00\n"
        "10:00 1 alice\n"
        "10:05 2 alice 1\n"
        "10:10 1 bob\n"
        "10:15 3 bob\n"
        "10:15 13 ICanWaitNoLonger!\n"
        "10:20 2 bob 2\n"
        "10:30 4 alice\n"
        "10:35 4 bob\n"
        "23:00\n"
        "1 100 00:25\n"
        "2 100 00:15\n";

    EXPECT_EQ(output, expected);
}

// Тест ошибки: нулевое количество столов
TEST_F(ClubManagerTest, ErrorInvalidNumTables)
{
    std::ofstream file("invalid_num_tables.txt");
    file << "0\n"
         << "09:00 23:00\n"
         << "100\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_num_tables.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "0\n");
}

// Тест ошибки: неверный формат времени
TEST_F(ClubManagerTest, ErrorInvalidTimeFormat)
{
    std::ofstream file("invalid_time.txt");
    file << "3\n"
         << "09:00 25:67\n"
         << "100\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_time.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "09:00 25:67\n");
}

// Тест ошибки: время открытия позже времени закрытия
TEST_F(ClubManagerTest, ErrorInvalidTimeOrder)
{
    std::ofstream file("invalid_time.txt");
    file << "3\n"
         << "14:00 09:30\n"
         << "100\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_time.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "14:00 09:30\n");
}

// Тест ошибки: нулевая почасовая ставка
TEST_F(ClubManagerTest, ErrorZeroHourlyRate)
{
    std::ofstream file("invalid_hourly_rate.txt");
    file << "3\n"
         << "09:00 18:30\n"
         << "0\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_hourly_rate.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "0\n");
}

// Тест ошибки: отрицательная почасовая ставка
TEST_F(ClubManagerTest, ErrorNegativeHourlyRate)
{
    std::ofstream file("invalid_hourly_rate.txt");
    file << "3\n"
         << "09:00 18:30\n"
         << "-100\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_hourly_rate.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "-100\n");
}

// Тест ошибки: невалидное время события
TEST_F(ClubManagerTest, ErrorInvalidEventTime)
{
    std::ofstream file("invalid_event_time.txt");
    file << "3\n"
         << "09:00 18:30\n"
         << "100\n"
         << "10:00 1 alice\n"
         << "10:05 2 alice 1\n"
         << "10:89 1 bob\n"
         << "10:15 3 bob\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_event_time.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "10:89 1 bob\n");
}

// Тест ошибки: невалидное имя клиента
TEST_F(ClubManagerTest, ErrorInvalidClientName)
{
    std::ofstream file("invalid_client_name.txt");
    file << "3\n"
         << "09:00 18:30\n"
         << "100\n"
         << "10:00 1 alice\n"
         << "10:05 2 alice 1\n"
         << "10:10 1 bob*&^\n"
         << "10:15 3 bob\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_client_name.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "10:10 1 bob*&^\n");
}

// Тест ошибки: невалидный ID события
TEST_F(ClubManagerTest, ErrorInvalidEventID)
{
    std::ofstream file("invalid_event_id.txt");
    file << "3\n"
         << "09:00 18:30\n"
         << "100\n"
         << "10:00 1 alice\n"
         << "10:05 17 alice 1\n"
         << "10:10 1 bob\n"
         << "10:15 3 bob\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_event_id.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "10:05 17 alice 1\n");
}

// Тест ошибки: невалидный ID стола
TEST_F(ClubManagerTest, ErrorInvalidTableID)
{
    std::ofstream file("invalid_table_id.txt");
    file << "3\n"
         << "09:00 18:30\n"
         << "100\n"
         << "10:00 1 alice\n"
         << "10:05 2 alice 4\n"
         << "10:10 1 bob\n"
         << "10:15 3 bob\n";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("invalid_table_id.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "10:05 2 alice 4\n");
}

// Тест ошибки: пустой файл
TEST_F(ClubManagerTest, ErrorEmptyFile)
{
    std::ofstream file("empty_file.txt");
    file << "";
    file.close();

    std::ofstream cout_file("cout_output.txt");
    std::streambuf *cout_old = std::cout.rdbuf(cout_file.rdbuf());

    EXPECT_EXIT(
        {
            ClubManager manager("empty_file.txt");
            manager.run();
            std::cout.flush();
        },
        testing::ExitedWithCode(1),
        "");

    std::cout.rdbuf(cout_old);
    cout_file.close();

    std::ifstream result_file("cout_output.txt");
    std::stringstream buffer;
    buffer << result_file.rdbuf();
    std::string cout_output = buffer.str();
    result_file.close();
    std::remove("cout_output.txt");

    EXPECT_EQ(cout_output, "Empty file\n");
}

