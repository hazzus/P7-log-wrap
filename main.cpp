#include <iostream>

#include "logger.h"

int main() {
    Logger file_log(Logger::Output::Console);
    try {
        file_log.init("Worker", "Main", "logs");
    } catch (std::runtime_error& e) {
        std::cerr << "Error raised: " << e.what() << ", exiting " << std::endl;
        return 1;
    }
    file_log.info("Hello, log!");
    int x = 4;
    file_log.debug("Some debug information x: %d", 5);
    return 0;
}
