#include <gtest/gtest.h>
#include "ClubConfig.hpp"
#include "Time.hpp"

class ClubConfigTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        valid_config_ = std::make_unique<ClubConfig>(
            5, Time("09:00"), Time("22:00"), 100);
    }

    std::unique_ptr<ClubConfig> valid_config_;
};

TEST_F(ClubConfigTest, ValidConfig)
{
    EXPECT_EQ(valid_config_->get_num_tables(), 5);
    EXPECT_EQ(valid_config_->get_start_time().to_string(), "09:00");
    EXPECT_EQ(valid_config_->get_end_time().to_string(), "22:00");
    EXPECT_EQ(valid_config_->get_hourly_rate(), 100);
}

TEST_F(ClubConfigTest, OperatingHours)
{
    EXPECT_TRUE(valid_config_->is_within_open_hours(Time("09:00")));
    EXPECT_TRUE(valid_config_->is_within_open_hours(Time("15:00")));
    EXPECT_TRUE(valid_config_->is_within_open_hours(Time("22:00")));
    EXPECT_FALSE(valid_config_->is_within_open_hours(Time("08:59")));
    EXPECT_FALSE(valid_config_->is_within_open_hours(Time("22:01")));
}