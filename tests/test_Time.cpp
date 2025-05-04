#include <gtest/gtest.h>
#include "Time.hpp"

TEST(TimeTest, DefaultConstructor) {
    Time t;
    EXPECT_EQ(t.to_string(), "00:00");
    EXPECT_EQ(t.get_hours(), 0);
    EXPECT_EQ(t.get_minutes(), 0);
}

TEST(TimeTest, ParseValidTime) {
    Time t("10:30");
    EXPECT_EQ(t.to_string(), "10:30");
    EXPECT_EQ(t.get_hours(), 10);
    EXPECT_EQ(t.get_minutes(), 30);
}

TEST(TimeTest, InvalidTimeThrows) {
    EXPECT_THROW(Time("25:00"), std::invalid_argument);
    EXPECT_THROW(Time("10:60"), std::invalid_argument);
    EXPECT_THROW(Time("abc"), std::invalid_argument);
}

TEST(TimeTest, Comparison) {
    Time t1("10:00");
    Time t2("10:30");
    Time t3("11:00");
    EXPECT_TRUE(t1 < t2);
    EXPECT_TRUE(t2 <= t3);
    EXPECT_TRUE(t1 == Time("10:00"));
    EXPECT_FALSE(t1 == t2);
}
