#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <string>
#include <iostream>

class Logger {
private:
    static const std::string PREFIX;

    static const std::string COLOR_RED;
    static const std::string COLOR_GREEN;
    static const std::string COLOR_RESET;
    static const std::string COLOR_YELLOW;

public:
    static void info(const std::string& text);
    static void error(const std::string& text);
    static void debug(const std::string& text);
};

#endif // LOGGER_H