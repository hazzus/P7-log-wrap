#ifndef LOGGER_H
#define LOGGER_H

#include <assert.h>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "P7_Client.h"
#include "P7_Trace.h"

struct operable;

static int line = 0;

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

    void info(const char* str, ...);

    void debug(const char* str, ...);

    void warning(const char* str, ...);

    void error(const char* str, ...);

    void critical(const char* str, ...);

    template <typename T>
    friend inline operable&& operator<<(operable&& op, T&& val);

  private:
    static Logger* _instance;

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

template <typename T> operable&& operator<<(operable&& op, T&& val) {
    op.logger->buffer_level = op.level;
    std::string str;
    if constexpr (std::is_convertible<T, std::string>::value) {
        str = val;
    } else {
        str = std::to_string(val);
    }
    if (op.level == EP7TRACE_LEVEL_INFO) {
        op.logger->info(str.c_str());
    } else if (op.level == EP7TRACE_LEVEL_DEBUG) {
        op.logger->debug(str.c_str());
    }
    op.logger->shift_buffer += " ";
    return std::move(op);
}

inline operable INFO() { return {EP7TRACE_LEVEL_INFO, Logger::instance()}; }
inline operable DEBUG() { return {EP7TRACE_LEVEL_DEBUG, Logger::instance()}; }
inline operable WARNING() {
    return {EP7TRACE_LEVEL_WARNING, Logger::instance()};
}
inline operable ERROR() { return {EP7TRACE_LEVEL_ERROR, Logger::instance()}; }
inline operable CRITICAL() {
    return {EP7TRACE_LEVEL_CRITICAL, Logger::instance()};
}

#endif // LOGGER_H
