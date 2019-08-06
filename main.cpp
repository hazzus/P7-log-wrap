#include <iomanip>
#include <iostream>

#include "logger.h"

int main() {
    Logger& logger = Logger::instance();
    logger.setFlag(Logger::Output::Console);
    logger.setFlag(Logger::Output::File, "logs");
    try {
        logger.init("Main thread", "Main");
    } catch (std::runtime_error& e) {
        std::cerr << "Error occured: " << e.what() << ". Exiting..."
                  << std::endl;
        return 1;
    }
    int x = rand();
    logger.info("Hello, friend. Your random number: %d", x);
    logger << "Info is default for operator output";
    DEBUG() << "Some operator debug info";
    logger << EP7TRACE_LEVEL_WARNING;
    logger << "More functionalities appear!";
}
