#include <iomanip>
#include <iostream>

#include "logger.h"

void call_me_baby() {
    Logger* log = Logger::instance();

    //Это основной use case

    // BUG only first phrase with same type is outputing, library optimisation?
    // no such use case actually can be used
    // DEBUG() << "Hello" << 343 << "Goodbye";
    // INFO() << "WOW" << 666;

    log->warning("Omg %d %s", 4, "omg");
    try {
        log->setFlag(
            Logger::Output::Network,
            true); // log скрывает за собой Logger::instance(). При
                   // передаче флага указываем включить его или выключить

        log->setFlag(Logger::Output::File, false); // Выключаем запись в файл

        log->setFilePath("path"); // Изменяем файл для логов

        log->setServer("ip"); // Изменяем адрес сервера
    } catch (std::runtime_error& e) {
        std::cerr << "Error occured: " << e.what() << ". Exiting..."
                  << std::endl;
    }
    log->info("Hello from another log");
    log->debug("No way %d", 4);
}

int main() {
    Logger log(Logger::Output::Console | Logger::Output::File);
    try {
        log.init("logs", "127.0.0.1");
    } catch (std::runtime_error& e) {
        std::cerr << "Error. " << e.what() << ". Exiting..." << std::endl;
        return 1;
    }

    call_me_baby();
}
