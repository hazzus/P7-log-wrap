#include "logger.h"

Logger* Logger::_instance = nullptr;

Logger::Logger() : flags(0) { _instance = this; }

Logger::Logger(Output flags) : flags(static_cast<int>(flags)) {
    _instance = this;
}

void Logger::init(const std::string& log_dir, const std::string& baical_addr) {
    _output[0].args = "/P7.Sink=Console";
    _output[1].args = "/P7.Sink=FileTxt /P7.Dir=" + log_dir;
    _output[2].args = "/P7.Sink=Baical /P7.Addr=" + baical_addr;

    if (flags & static_cast<int>(Output::Console)) {
        _output[0].activate();
    }
    if (flags & static_cast<int>(Output::File)) {
        _output[1].activate();
    }
    if (flags & static_cast<int>(Output::Network)) {
        _output[2].activate();
    }
    info("Starting logging");
}

void Logger::setFlag(Output flag, const bool value) {
    if (flag == Output::Console) {
        _output[0].reset(value);
    } else if (flag == Output::File) {
        _output[1].reset(value);
    } else {
        _output[2].reset(value);
    }
}

void Logger::setFilePath(const std::string& filepath) {
    _output[1].args = "/P7.Sink=FileTxt /P7.Dir=" + filepath;
    _output[1].activate();
}

void Logger::setServer(const std::string& ip) {
    _output[2].args = "/P7.Sink=Baical /P7.Addr=" + ip;
    _output[2].activate();
}

void Logger::info(const char* str, ...) {
    va_list args;
    va_start(args, str);
    for (stream const& s : _output) {
        if (s.enabled) {
            s.trace->Trace_Embedded(0, EP7TRACE_LEVEL_INFO, nullptr, 0, nullptr,
                                    nullptr, &str, &args);
        }
    }
    va_end(args);
}

void Logger::debug(const char* str, ...) {
    va_list args;
    va_start(args, str);
    for (stream const& s : _output) {
        if (s.enabled) {
            s.trace->Trace_Embedded(0, EP7TRACE_LEVEL_DEBUG, nullptr, 0,
                                    nullptr, nullptr, &str, &args);
        }
    }
    va_end(args);
}

void Logger::warning(const char* str, ...) {
    va_list args;
    va_start(args, str);
    for (stream const& s : _output) {
        if (s.enabled) {
            s.trace->Trace_Embedded(0, EP7TRACE_LEVEL_WARNING, nullptr, 0,
                                    nullptr, nullptr, &str, &args);
        }
    }
    va_end(args);
}

void Logger::error(const char* str, ...) {
    va_list args;
    va_start(args, str);
    for (stream const& s : _output) {
        if (s.enabled) {
            s.trace->Trace_Embedded(0, EP7TRACE_LEVEL_ERROR, nullptr, 0,
                                    nullptr, nullptr, &str, &args);
        }
    }
    va_end(args);
}

void Logger::critical(const char* str, ...) {
    va_list args;
    va_start(args, str);
    for (stream const& s : _output) {
        if (s.enabled) {
            s.trace->Trace_Embedded(0, EP7TRACE_LEVEL_CRITICAL, nullptr, 0,
                                    nullptr, nullptr, &str, &args);
        }
    }
    va_end(args);
}

Logger::~Logger() {
    info("Stopping logging");
    for (auto& s : _output) {
        s.release();
    }
}

Logger* Logger::instance() {
    assert(_instance != nullptr);
    return _instance;
}

Logger::stream::stream()
    : enabled(false), args(), client(nullptr), trace(nullptr) {}

void Logger::stream::activate() {
    IP7_Client* next_client = P7_Create_Client(TM(args.c_str()));
    if (next_client == nullptr) {
        throw std::runtime_error("Cannot create client");
    }
    IP7_Trace* next_trace = P7_Create_Trace(next_client, TM("Testing trace"));
    if (next_trace == nullptr) {
        throw std::runtime_error("Cannot create trace");
    }

    release();
    client = std::move(next_client);
    trace = std::move(next_trace);

    enabled = true;
}

void Logger::stream::release() {
    if (trace != nullptr) {
        trace->Release();
        trace = nullptr;
    }
    if (client != nullptr) {
        client->Release();
        client = nullptr;
    }
}

void Logger::stream::reset(const bool value) {
    if (enabled == value)
        return;
    enabled = value;
    if (enabled) {
        activate();
    } else {
        release();
    }
}

Logger::stream::~stream() { release(); }
