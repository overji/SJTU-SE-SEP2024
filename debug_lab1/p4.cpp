#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "def.h"

class LogEntry {
public:
    LogEntry(
        std::chrono::system_clock::time_point timestamp,
        LogLevel level,
        std::string message,
        std::string source
    ) :
        timestamp(timestamp),
        level(level),
        message(std::move(message)),
        source(std::move(source)) {}

    std::chrono::system_clock::time_point get_timestamp() const {
        return timestamp;
    }

    LogLevel get_level() const {
        return level;
    }

    std::string get_message() const {
        return message;
    }

    std::string get_source() const {
        return source;
    }

private:
    std::chrono::system_clock::time_point timestamp;
    LogLevel level;
    std::string message;
    std::string source;
};

class LogAnalyzer {
public:
    void add_log_entry(const LogEntry &entry) {
        logs.push_back(entry);
    }

    void
    remove_old_logs(const std::chrono::system_clock::time_point &cutoff_time) {
        for (auto it = logs.begin(); it != logs.end(); it++) {
            if (it->get_timestamp() < cutoff_time) {
                logs.erase(it);
            }
            if(it == logs.end()){
                break;
            }
        }
    }

    void remove_info_logs() {
        for (auto it = logs.begin(); it != logs.end(); it++) {
            if (it->get_level() == LogLevel::INFO
                || it->get_level() == LogLevel::DEBUG) {
                logs.erase(it);
            }
            if(it == logs.end()){
                break;
            }
        }
    }

    std::map<std::string, int> get_error_count_by_source() const {
        std::map<std::string, int> error_count;
        for (const auto &log : logs) {
            if (log.get_level() == LogLevel::ERROR
                || log.get_level() == LogLevel::CRITICAL) {
                error_count[log.get_source()]++;
            }
        }
        return error_count;
    }

    void print_log_summary() const {
        std::map<LogLevel, int> level_count;
        for (const auto &log : logs) {
            level_count[log.get_level()]++;
        }

        std::cout << "Log Summary:" << std::endl;
        std::cout << "  Total Logs: " << logs.size() << std::endl;
        std::cout << "  Debug: " << level_count[LogLevel::DEBUG] << std::endl;
        std::cout << "  Info: " << level_count[LogLevel::INFO] << std::endl;
        std::cout << "  Warning: " << level_count[LogLevel::WARNING]
                  << std::endl;
        std::cout << "  Error: " << level_count[LogLevel::ERROR] << std::endl;
        std::cout << "  Critical: " << level_count[LogLevel::CRITICAL]
                  << std::endl;
    }

    void print_recent_errors(int n) const {
        std::cout << "Recent Errors:" << std::endl;
        int count = 0;
        for (auto it = logs.rbegin(); it != logs.rend() && count < n; ++it) {
            if (it->get_level() == LogLevel::ERROR
                || it->get_level() == LogLevel::CRITICAL) {
                std::cout << "  " << level_to_string(it->get_level()) << " ["
                          << time_point_to_string(it->get_timestamp()) << "] "
                          << it->get_source() << ": " << it->get_message()
                          << std::endl;
                count++;
            }
        }
    }

private:
    std::vector<LogEntry> logs;

    static std::string
    time_point_to_string(const std::chrono::system_clock::time_point &tp) {
        auto time = std::chrono::system_clock::to_time_t(tp);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    static std::string level_to_string(LogLevel level) {
        switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }
};

int main() {
    LogAnalyzer analyzer;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string timestamp_str, level_str, source, message;

        if (std::getline(iss, timestamp_str, '|')
            && std::getline(iss, level_str, '|')
            && std::getline(iss, source, '|') && std::getline(iss, message)) {
            std::tm tm = {};
            std::istringstream ss(timestamp_str);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            auto timestamp =
                std::chrono::system_clock::from_time_t(std::mktime(&tm));
            LogLevel level;
            if (level_str == "DEBUG") {
                level = LogLevel::DEBUG;
            } else if (level_str == "INFO") {
                level = LogLevel::INFO;
            } else if (level_str == "WARNING") {
                level = LogLevel::WARNING;
            } else if (level_str == "ERROR") {
                level = LogLevel::ERROR;
            } else if (level_str == "CRITICAL") {
                level = LogLevel::CRITICAL;
            } else {
                continue; // Invalid log level, skip this entry
            }

            analyzer.add_log_entry(LogEntry(timestamp, level, message, source));
        }
    }

    analyzer.print_log_summary();

    std::cout << "\nRemoving logs older than 7 days..." << std::endl;
    auto cutoffTime =
        std::chrono::system_clock::now() - std::chrono::hours(7 * 24);
    analyzer.remove_old_logs(cutoffTime);

    std::cout << "Log summary after removing old logs:" << std::endl;
    analyzer.print_log_summary();

    std::cout << "\nRemoving INFO and DEBUG logs..." << std::endl;
    analyzer.remove_info_logs();

    std::cout << "Log summary after removing INFO and DEBUG logs:" << std::endl;
    analyzer.print_log_summary();

    std::cout << "\nError count by source:" << std::endl;
    auto errorCountBySource = analyzer.get_error_count_by_source();
    for (const auto &pair : errorCountBySource) {
        const auto &source = pair.first;
        const auto &count = pair.second;
        std::cout << "  " << source << ": " << count << std::endl;
    }

    std::cout << "\nMost recent errors:" << std::endl;
    analyzer.print_recent_errors(5);

    return 0;
}
