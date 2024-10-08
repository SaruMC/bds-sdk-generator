#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <string>
#include <iostream>

class Logger {
private:
    static const std::string PREFIX;

public:
    static void info(const std::string& text);
    static void error(const std::string& text);
    static void debug(const std::string& text);
};

#endif // LOGGER_H