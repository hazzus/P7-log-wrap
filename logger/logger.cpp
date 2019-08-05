#include "logger.h"

#include <assert.h>

Logger::Logger()
    : flag(Output::Null), trace(nullptr,
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

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

// TODO many outputs same time (needs massive refactor)
void Logger::setFlag(Output out) { flag = out; }

void Logger::init(std::string const& thread_name,
                  std::string const& module_name,
                  std::string const& dir_or_addr) {
    // TODO maybe check and block re-init then no strong exception safety needed
    std::string create_call;
    if (flag == Output::Null) {
        throw std::runtime_error("No output flag has been set for logger");
    } else if (flag == Output::Console) {
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

Logger::~Logger() { trace->P7_INFO(module, TM("Log closing")); }
