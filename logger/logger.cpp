#include "logger.h"

#include <assert.h>

Logger::Logger(Output out)
    : flag(out), trace(nullptr,
                       [](IP7_Trace* trace) {
                           if (trace != nullptr) {
                               trace->Unregister_Thread(0);
                               trace->Release();
                           }
                       }),
      client(nullptr, [](IP7_Client* client) {
          if (client != nullptr) {
              client->Release();
          }
      }) {}

void Logger::init(std::string const& thread_name,
                  std::string const& module_name,
                  std::string const& dir_or_addr) {
    // TODO maybe check and block re-init then no strong exception safety needed
    std::string create_call;
    if (flag == Output::Console) {
        create_call = "/P7.Sink=Console";
    } else if (flag == Output::File) {
        create_call = "/P7.Sink=FileTxt /P7.Dir=" + dir_or_addr;
    } else {
        create_call = "/P7.Sink=Baical /P7.Addr=" + dir_or_addr;
    }
    IP7_Client* n_client = P7_Create_Client(TM(create_call.c_str()));
    if (n_client == nullptr) {
        throw std::runtime_error("Cannot create client");
    }
    IP7_Trace* n_trace = P7_Create_Trace(n_client, TM("Test log"));
    if (n_trace == nullptr) {
        throw std::runtime_error("Cannot create trace");
    }
    client.reset(n_client);
    trace.reset(n_trace);

    trace->Register_Thread(TM(thread_name.c_str()), 0);
    trace->Register_Module(TM(module_name.c_str()), &module);

    trace->P7_INFO(module, "Starting logging");
}

// TODO variadic arguments for functions
void Logger::debug(const std::string& str) { log(EP7TRACE_LEVEL_DEBUG, str); }

void Logger::info(const std::string& str) { log(EP7TRACE_LEVEL_INFO, str); }

void Logger::warning(const std::string& str) {
    log(EP7TRACE_LEVEL_WARNING, str);
}

void Logger::error(const std::string& str) { log(EP7TRACE_LEVEL_ERROR, str); }

void Logger::critical(const std::string& str) {
    log(EP7TRACE_LEVEL_CRITICAL, str);
}

void Logger::log(eP7Trace_Level level, const std::string& str) {
    trace->P7_DELIVER(0, level, module, TM(str.c_str()));
}

Logger::~Logger() { trace->P7_INFO(module, TM("Log closing")); }
