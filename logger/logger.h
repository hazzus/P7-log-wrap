#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <unordered_map>

#include "P7_Client.h"
#include "P7_Trace.h"

class Logger {
  public:
    struct Output {
        static const int Console = 1, File = 2, Network = 4;
    };

    Logger();
    Logger(int const flags);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(const Logger&&) = delete;

    static Logger* instance();

    void init(std::string const& log_dir, std::string const& baical_addr);

    void setFlag(int const flag, bool const value);

    void setFilePath(std::string const& filepath);

    void setServer(std::string const& ip);

    template <typename... T> void debug(const std::string& str, T... args) {
        log(EP7TRACE_LEVEL_DEBUG, str, args...);
    }

    template <typename... T> void info(const std::string& str, T... args) {
        log(EP7TRACE_LEVEL_INFO, str, args...);
    }

    template <typename... T> void warning(const std::string& str, T... args) {
        log(EP7TRACE_LEVEL_WARNING, str, args...);
    }

    template <typename... T> void error(const std::string& str, T... args) {
        log(EP7TRACE_LEVEL_ERROR, str, args...);
    }

    template <typename... T> void critical(const std::string& str, T... args) {
        log(EP7TRACE_LEVEL_CRITICAL, str, args...);
    }

  private:
    template <typename... T>
    void log(eP7Trace_Level const& level, std::string const& str, T... args) {
        for (stream const& s : _output) {
            if (s.enabled) {
                s.trace->P7_DELIVER(0, level, nullptr, TM(str.c_str()),
                                    args...);
            }
        }
    }

    struct stream {
      public:
        stream();

        void activate();
        void release();
        void reset(bool const value);

        ~stream();

        bool enabled;
        std::string args;
        IP7_Client* client;
        IP7_Trace* trace;
    };

    int flags;
    stream _output[3];
};

struct operable {
    eP7Trace_Level level;
    Logger* logger;
};

#endif // LOGGER_H
