#include <iomanip>
#include <iostream>

#include "logger.h"

void call_me_baby() {
    Logger* log = Logger::instance();

    //Это основной use case

    // BUG only first phrase is outputing, but why?
    DEBUG() << "Hello" << 343 << "Goodbye";

    log->warning("Hello %d %s", 343, "Goodbye");

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
}

int main() {
    Logger log(Logger::Output::Console | Logger::Output::File);
    log.init("logs", "127.0.0.1");

    call_me_baby();
}
