#include "pid_manager.h"
#include <gtest/gtest.h>

TEST(PidManagerTest, AllocateMapSuccess) { EXPECT_EQ(allocate_map(), 1); }

TEST(PidManagerTest, AllocatePidSuccess) {
    allocate_map();
    int pid = allocate_pid();
    EXPECT_GE(pid, MIN_PID);
    EXPECT_LE(pid, MAX_PID);
}

TEST(PidManagerTest, AllocatePidFailure) {
    allocate_map();
    for (int i = 0; i < MAX_PID - MIN_PID + 1; i++) {
        allocate_pid();
    }
    EXPECT_EQ(allocate_pid(), -1);
}

TEST(PidManagerTest, ReallocatePid) {
    allocate_map();
    int pid1 = allocate_pid();
    EXPECT_GE(pid1, MIN_PID);
    EXPECT_LE(pid1, MAX_PID);

    release_pid(pid1);

    int pid2 = allocate_pid();
    EXPECT_EQ(pid1, pid2);

    // Additional checks to ensure proper behavior after reallocation
    release_pid(pid2);
    int pid3 = allocate_pid();
    EXPECT_EQ(pid2, pid3);

    // Ensure no allocation occurs when all PIDs are released and reallocated
    release_pid(pid3);
    for (int i = 0; i < MAX_PID - MIN_PID + 1; i++) {
        EXPECT_NE(allocate_pid(), -1);
    }
    EXPECT_EQ(allocate_pid(), -1);
}

TEST(PidManagerTest, AllocateAndReleaseAllPids) {
    allocate_map();

    // Allocate all PIDs
    for (int i = 0; i < MAX_PID - MIN_PID + 1; i++) {
        EXPECT_NE(allocate_pid(), -1);
    }

    // Ensure no more PIDs can be allocated
    EXPECT_EQ(allocate_pid(), -1);

    // Release all PIDs
    for (int i = MIN_PID; i <= MAX_PID; i++) {
        release_pid(i);
    }

    // Ensure PIDs can be reallocated
    for (int i = 0; i < MAX_PID - MIN_PID + 1; i++) {
        EXPECT_NE(allocate_pid(), -1);
    }

    // Ensure no more PIDs can be allocated again
    EXPECT_EQ(allocate_pid(), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}