#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>

#include "P7_Client.h"
#include "P7_Trace.h"

class Logger {
  public:
    enum class Output { Console, File, Network, Null };

    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(const Logger&&) = delete;

    static Logger& instance();

    void setFlag(Output out);

    void init(std::string const& thread_name = "Default thread name",
              std::string const& module_name = "Default module name",
              std::string const& dir_or_addr = "");

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
    void log(eP7Trace_Level level, const std::string& str, T... args) {
        trace->P7_DELIVER(0, level, module, TM(str.c_str()), args...);
    }

    Output flag = Output::Console;
    std::unique_ptr<IP7_Trace, void (*)(IP7_Trace*)> trace;
    std::unique_ptr<IP7_Client, void (*)(IP7_Client*)> client;
    IP7_Trace::hModule module = nullptr;
};
#endif // LOGGER_H
