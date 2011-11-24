#include "logger.h"

Console* Logger::console;
int Logger::ctor = 0;
int Logger::dtor = 0;
std::ofstream Logger::logStream;
std::ofstream Logger::structStream;

Logger::Logger(const std::string& moduleName) {
    name = moduleName;
}

std::ofstream& Logger::getStructStream() {
    // Get the current calendar time as a time_t object
    time_t now = time(0);
    // Convert time_t to tm as local time
    tm local = *localtime(&now);

    // Format time to "hh:mm:ss" string
    char timeString[50];
    const char format[] = "%X";
    strftime(timeString, sizeof(timeString) - 1, format, &local);
    structStream << timeString << ": " << std::setw(22) << std::left << name << ": ";

    return structStream;
}

std::ofstream& Logger::getStructStream(bool ctor) {
    if (ctor)
       Logger::ctor++;
    else
       Logger::dtor++;

    return Logger::getStructStream();
}
std::ofstream& Logger::getStream() {
    // Get the current calendar time as a time_t object
    time_t now = time(0);
    // Convert time_t to tm as local time
    tm local = *localtime(&now);

    // Format time to "hh:mm:ss" string
    char timeString[50];
    const char format[] = "%X";
    strftime(timeString, sizeof(timeString) - 1, format, &local);
    logStream << timeString << ": " << std::setw(22) << std::left << name << ": ";
    return logStream;
}

void Logger::setConsole(Console& c) {
    console = &c;
}

void Logger::toConsole(const std::string line) {
    console->log(line);
}

//Opens new log file
void Logger::start(const std::string& robot) {
    time_t now = time(0);
    tm local = *localtime(&now);
    const char* fileName1;
    const char* fileName2;
    std::string name1;
    std::string name2;
    std::stringstream buildDate;
    buildDate << (local.tm_year+1900) << (local.tm_mon+1) <<  local.tm_mday;
    name1 = "log/" + robot + "." + buildDate.str() + ".log";
    name2 = "log/" + robot + "Struct." + buildDate.str() + ".log";
    fileName1 = name1.c_str();
    fileName2 = name2.c_str();

    logStream.open(fileName1, std::ios::app);
    logStream << "Start log - " << local.tm_hour<< ":" << local.tm_min << ":" << local.tm_sec << std::endl;
    structStream.open(fileName2, std::ios::app);
    structStream << "Start log - " << local.tm_hour<< ":" << local.tm_min << ":" << local.tm_sec << std::endl;
}

//Closes log file
void Logger::stop() {
    logStream.close();

    structStream << "Constructed: " << Logger::ctor << " "
                 << "Destructed: " << Logger::dtor << std::endl;

    structStream.close();
}
