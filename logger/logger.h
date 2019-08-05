#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>

#include "P7_Client.h"
#include "P7_Trace.h"

class Logger {
  public:
    enum class Output { Console, File, Network };

    Logger(Output out = Output::Console);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(const Logger&&) = delete;

    static Logger* instance();

    void init(std::string const& thread_name = "Default thread name",
              std::string const& module_name = "Default module name",
              std::string const& dir_or_addr = "");

    void debug(const std::string& str);

    void info(const std::string& str);

    void warning(const std::string& str);

    void error(const std::string& str);

    void critical(const std::string& str);

  private:
    void log(eP7Trace_Level level, const std::string& str);

    Output flag = Output::Console;
    std::unique_ptr<IP7_Trace, void (*)(IP7_Trace*)> trace;
    std::unique_ptr<IP7_Client, void (*)(IP7_Client*)> client;
    IP7_Trace::hModule module = nullptr;
};
#endif // LOGGER_H
