#include "pid_manager.h"
#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <random>
#include <set>
#include <thread>
#include <vector>

std::vector<int> allocated_pids;
std::mutex pid_vec_mutex;

void thread_pid_task(int thread_id) {
    int pid = allocate_pid();
    if (pid == -1) {
        std::cerr << "Thread " << thread_id << ": Failed to allocate PID\n";
        return;
    }
    {
        std::lock_guard<std::mutex> lock(pid_vec_mutex);
        allocated_pids.push_back(pid);
    }
    // Sleep for 1-3 seconds randomly
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(1, 3);
    int sleep_time = dist(gen);
    std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    release_pid(pid);
    // Optionally print for debugging
    // std::cout << "Thread " << thread_id << ": Released PID " << pid << "\n";
}

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

TEST(PidManagerTest, MultithreadedPidAllocation) {
    allocate_map();
    allocated_pids.clear();
    const int num_threads = 100;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_pid_task, i);
    }
    for (auto &t : threads) {
        t.join();
    }
    // 檢查所有分配到的 PID 是否唯一且合法
    std::set<int> pid_set;
    for (int pid : allocated_pids) {
        EXPECT_GE(pid, MIN_PID);
        EXPECT_LE(pid, MAX_PID);
        pid_set.insert(pid);
    }
    EXPECT_EQ(pid_set.size(), allocated_pids.size())
        << "Duplicate PID detected!";

    // After all threads, all PIDs should be released and available again
    for (int i = 0; i < MAX_PID - MIN_PID + 1; i++) {
        EXPECT_NE(allocate_pid(), -1);
    }
    EXPECT_EQ(allocate_pid(), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}