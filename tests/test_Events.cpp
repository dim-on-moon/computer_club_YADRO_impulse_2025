#include <gtest/gtest.h>
#include "Events.hpp"

class IEventTest : public ::testing::Test {
protected:
    void SetUp() override {
        time_ = Time("10:00");
    }

    Time time_;
};

TEST_F(IEventTest, ClientArrivedEvent) {
    ClientArrivedEvent event(time_, "client1");
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 1);
    EXPECT_EQ(event.get_client_name(), "client1");
    EXPECT_EQ(event.to_string(), "10:00 1 client1");
}

TEST_F(IEventTest, ClientSatEvent) {
    ClientSatEvent event(time_, "client1", 1);
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 2);
    EXPECT_EQ(event.get_client_name(), "client1");
    EXPECT_EQ(event.get_table_id(), 1);
    EXPECT_EQ(event.to_string(), "10:00 2 client1 1");
}

TEST_F(IEventTest, ClientWaitingEvent) {
    ClientWaitingEvent event(time_, "client1");
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 3);
    EXPECT_EQ(event.get_client_name(), "client1");
    EXPECT_EQ(event.to_string(), "10:00 3 client1");
}

TEST_F(IEventTest, ClientLeftEvent) {
    ClientLeftEvent event(time_, "client1");
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 4);
    EXPECT_EQ(event.get_client_name(), "client1");
    EXPECT_EQ(event.to_string(), "10:00 4 client1");
}

TEST_F(IEventTest, ClientLeftEndOfDayEvent) {
    ClientLeftEndOfDayEvent event(time_, "client1");
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 11);
    EXPECT_EQ(event.get_client_name(), "client1");
    EXPECT_EQ(event.to_string(), "10:00 11 client1");
}

TEST_F(IEventTest, ClientSatFromQueueEvent) {
    ClientSatFromQueueEvent event(time_, "client1", 2);
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 12);
    EXPECT_EQ(event.get_client_name(), "client1");
    EXPECT_EQ(event.get_table_id(), 2);
    EXPECT_EQ(event.to_string(), "10:00 12 client1 2");
}

TEST_F(IEventTest, ErrorEvent) {
    ErrorEvent event(time_, "Invalid_input");
    EXPECT_EQ(event.get_time().to_string(), "10:00");
    EXPECT_EQ(event.get_id(), 13);
    EXPECT_EQ(event.get_message(), "Invalid_input");
    EXPECT_EQ(event.to_string(), "10:00 13 Invalid_input");
}