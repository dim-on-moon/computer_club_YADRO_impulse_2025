#include <gtest/gtest.h>
#include "Client.hpp"

class ClientTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        client_ = std::make_unique<Client>("client1");
    }

    std::unique_ptr<Client> client_;
};

TEST_F(ClientTest, Constructor) {
    EXPECT_EQ(client_->get_name(), "client1");
    EXPECT_FALSE(client_->is_in_club());
    EXPECT_FALSE(client_->is_in_waiting());
    EXPECT_FALSE(client_->is_at_table());
    EXPECT_EQ(client_->get_table_id(), 0);
}

TEST_F(ClientTest, EnterClub) {
    client_->enter_club();
    EXPECT_TRUE(client_->is_in_club());
    EXPECT_FALSE(client_->is_in_waiting());
    EXPECT_FALSE(client_->is_at_table());
}

TEST_F(ClientTest, Wait) {
    client_->enter_club();
    client_->wait();
    EXPECT_TRUE(client_->is_in_club());
    EXPECT_TRUE(client_->is_in_waiting());
    EXPECT_FALSE(client_->is_at_table());
    EXPECT_EQ(client_->get_table_id(), 0);
}

TEST_F(ClientTest, SitAtTable) {
    client_->enter_club();
    client_->sit_at_table(1);
    EXPECT_TRUE(client_->is_in_club());
    EXPECT_FALSE(client_->is_in_waiting());
    EXPECT_TRUE(client_->is_at_table());
    EXPECT_EQ(client_->get_table_id(), 1);
}

TEST_F(ClientTest, LeaveClub) {
    client_->enter_club();
    client_->sit_at_table(1);
    client_->leave_club();
    EXPECT_FALSE(client_->is_in_club());
    EXPECT_FALSE(client_->is_in_waiting());
    EXPECT_FALSE(client_->is_at_table());
    EXPECT_EQ(client_->get_table_id(), 0);
}