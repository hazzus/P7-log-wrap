#include "logger.h"

Logger::Logger() {}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

void Logger::setFlag(Output out, std::string const& dir_or_addr) {
    flags[out] = dir_or_addr;
}

void Logger::init(std::string const& thread_name,
                  std::string const& module_name) {
    // TODO maybe check and block re-init then no strong exception safety needed
    if (flags.empty()) {
        throw std::runtime_error("No output flag has been set for logger");
    }
    if (flags.count(Output::Console)) {
        set_pair(0, "/P7.Sink=Console", thread_name, module_name);
    }
    auto f_it = flags.find(Output::File);
    if (f_it != flags.end()) {
        set_pair(1, "/P7.Sink=FileTxt /P7.Dir=" + f_it->second, thread_name,
                 module_name);
    }
    f_it = flags.find(Output::Network);
    if (f_it != flags.end()) {
        set_pair(2, "/P7.Sink=Baical /P7.Addr=" + f_it->second, thread_name,
                 module_name);
    }
    log(EP7TRACE_LEVEL_INFO, "Starting logging");
}

void Logger::set_pair(size_t const& index, std::string const& create_args,
                      std::string const& thread_name,
                      std::string const& module_name) {
    IP7_Client* client = P7_Create_Client(TM(create_args.c_str()));
    if (client == nullptr) {
        throw std::runtime_error("Couldn't create client");
    }
    IP7_Trace* trace = P7_Create_Trace(client, TM("Test trace"));
    if (trace == nullptr) {
        throw std::runtime_error("Couldn't create trace");
    }
    destruct_pair(client_traces[index]);
    client_traces[index].first = client;
    client_traces[index].second = trace;

    client_traces[index].second->Register_Thread(TM(thread_name.c_str()), 0);
    client_traces[index].second->Register_Module(TM(module_name.c_str()),
                                                 &module);
}

void Logger::destruct_pair(const std::pair<IP7_Client*, IP7_Trace*>& cl_tr) {
    if (cl_tr.second != nullptr) {
        cl_tr.second->Unregister_Thread(0);
        cl_tr.second->Release();
    }
    if (cl_tr.first != nullptr) {
        cl_tr.first->Release();
    }
}

Logger::~Logger() {
    log(EP7TRACE_LEVEL_INFO, "Closing log");
    for (auto const& p : client_traces) {
        destruct_pair(p);
    }
}
