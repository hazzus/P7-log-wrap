#ifndef LOGGER_H
#define LOGGER_H

#include <assert.h>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "P7_Client.h"
#include "P7_Trace.h"

struct operable;

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
        flush();
        log(EP7TRACE_LEVEL_DEBUG, str, args...);
    }

    template <typename... T> void info(const std::string& str, T... args) {
        flush();
        log(EP7TRACE_LEVEL_INFO, str, args...);
    }

    template <typename... T> void warning(const std::string& str, T... args) {
        flush();
        log(EP7TRACE_LEVEL_WARNING, str, args...);
    }

    template <typename... T> void error(const std::string& str, T... args) {
        flush();
        log(EP7TRACE_LEVEL_ERROR, str, args...);
    }

    template <typename... T> void critical(const std::string& str, T... args) {
        flush();
        log(EP7TRACE_LEVEL_CRITICAL, str, args...);
    }

    template <typename T> friend inline operable operator<<(operable op, T val);
    friend operable operator+(operable op, int val);

  private:
    static Logger* _instance;

    template <typename... T>
    void log(eP7Trace_Level const& level, std::string const& str, T... args) {
        for (stream const& s : _output) {
            if (s.enabled) {
                s.trace->P7_DELIVER(0, level, nullptr, TM(str.c_str()),
                                    args...);
            }
        }
    }

    void flush();

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

    std::string shift_buffer;
    eP7Trace_Level buffer_level;
};

struct operable {
    eP7Trace_Level level;
    Logger* logger;
};

template <typename T> operable operator<<(operable op, T val) {
    if (op.logger->buffer_level != op.level) {
        op.logger->flush();
    }
    op.logger->buffer_level = op.level;
    if constexpr (std::is_convertible<T, std::string>::value) {
        op.logger->shift_buffer += val;
    } else {
        op.logger->shift_buffer += std::to_string(val);
    }
    op.logger->shift_buffer += " ";
    return op;
}

operable INFO();
operable DEBUG();
operable WARNING();
operable ERROR();
operable CRITICAL();

#endif // LOGGER_H
