#include <iostream>

#include "logger.h"

int main() {
    Logger console_file_log;
    console_file_log.setFlag(Logger::Output::File, "logs");
    console_file_log.setFlag(Logger::Output::Console);
    try {
        console_file_log.init("Worker", "Main");
    } catch (std::runtime_error& e) {
        std::cerr << "Error raised: " << e.what() << ", exiting " << std::endl;
        return 1;
    }
    console_file_log.info("Hello, log!");
    int x = 6;
    console_file_log.debug("Some debug information x: %d", x);
    return 0;
}
