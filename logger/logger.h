#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "P7_Client.h"
#include "P7_Trace.h"

class Logger {
  public:
    enum class Output { Console = 1, File = 2, Network = 4 };

    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(const Logger&&) = delete;

    static Logger& instance();

    void setFlag(Output out, std::string const& dir_or_addr = "");

    void init(std::string const& thread_name = "Default thread name",
              std::string const& module_name = "Default module name");

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
        for (auto const& p : client_traces) {
            if (p.second != nullptr) {
                p.second->P7_DELIVER(0, level, module, TM(str.c_str()),
                                     args...);
            }
        }
    }

    void set_pair(size_t const& index, std::string const& create_args,
                  std::string const& thread_name,
                  std::string const& module_name);

    void destruct_pair(std::pair<IP7_Client*, IP7_Trace*> const& cl_tr);

    std::unordered_map<Output, std::string> flags;
    std::pair<IP7_Client*, IP7_Trace*> client_traces[3];
    IP7_Trace::hModule module = nullptr;
};
#endif // LOGGER_H
