#include "Logger.h"
#include <gtest/gtest.h>
#include <fstream>
#include <thread>
#include <filesystem>

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}

    const std::string testLogFile = "test_log.txt";
    const std::string testLogFileDir = LOG_DIR + "/";
};

TEST_F(LoggerTest, LogFileCreation) {
    Logger& logger = Logger::getInstance(testLogFile);
    logger.info("Testing log file creation");

    ASSERT_TRUE(std::filesystem::exists(testLogFileDir));
}

TEST_F(LoggerTest, LogContent) {
    Logger& logger = Logger::getInstance(testLogFile);
    std::string logStatement = "Test log content";
    bool found = false;
    std::string line;

    logger.info(logStatement);

    std::ifstream logFile(testLogFileDir + testLogFile);
    ASSERT_TRUE(logFile.is_open());

    while(getline(logFile, line)) {
        if(line.find(logStatement) != std::string::npos) {
            found = true;
        }
    }

    ASSERT_TRUE(found);
}

TEST_F(LoggerTest, ConcurrentLogging) {
    Logger& logger = Logger::getInstance(testLogFile);

    auto logTask = [&logger](const std::string& message) {
        for (int i = 0; i < 10; ++i) {
            logger.info(message);
        }
    };

    std::thread t1(logTask, "Thread 1");
    std::thread t2(logTask, "Thread 2");

    t1.join();
    t2.join();

    std::ifstream logFile(testLogFileDir + testLogFile);
    ASSERT_TRUE(logFile.is_open());

    int thread1Count = 0, thread2Count = 0;
    std::string line;
    while (std::getline(logFile, line)) {
        if (line.find("Thread 1") != std::string::npos) thread1Count++;
        if (line.find("Thread 2") != std::string::npos) thread2Count++;
    }

    ASSERT_EQ(thread1Count, 10);
    ASSERT_EQ(thread2Count, 10);
}