#include <gtest/gtest.h>
#include "Table.hpp"
#include "Time.hpp"

class TableTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        table_ = std::make_unique<Table>(1);
    }

    std::unique_ptr<Table> table_;
};

TEST_F(TableTest, Constructor)
{
    EXPECT_EQ(table_->get_id(), 1);
    EXPECT_FALSE(table_->is_occupied());
    EXPECT_EQ(table_->get_revenue(), 0);
    EXPECT_EQ(table_->get_total_occupied_time().to_string(), "00:00");
}

TEST_F(TableTest, OccupyAndFree)
{
    table_->occupy("client1", Time("10:00"));
    EXPECT_TRUE(table_->is_occupied());
    table_->free(Time("12:30"), 100);
    EXPECT_FALSE(table_->is_occupied());
    EXPECT_EQ(table_->get_revenue(), 300); // 2 часа 30 минут -> 3 часа * 100
    EXPECT_EQ(table_->get_total_occupied_time().to_string(), "02:30");
}

TEST_F(TableTest, MultipleSessions)
{
    table_->occupy("client1", Time("10:00"));
    table_->free(Time("11:00"), 100); // 1 час -> 100 руб
    EXPECT_EQ(table_->get_revenue(), 100);
    EXPECT_EQ(table_->get_total_occupied_time().to_string(), "01:00");

    table_->occupy("client2", Time("12:00"));
    table_->free(Time("13:45"), 100);      // 1 час 45 минут -> 2 часа * 100
    EXPECT_EQ(table_->get_revenue(), 300); // 100 + 200
    EXPECT_EQ(table_->get_total_occupied_time().to_string(), "02:45");
}