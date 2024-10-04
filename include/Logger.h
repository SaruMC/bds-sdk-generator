//
// Created by aldauge on 04/10/24.
//

#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <string>
#include <iostream>

class Logger {
private:
    static const std::string PREFIX;

public:
    static void info(std::string text) {
        std::cout << "\033[33m" << PREFIX << " - [INFO]: " << text << std::endl;
    }

    static void error(std::string text) {
        std::cerr << "\033[31m" << PREFIX << " - [ERROR]: " << text << std::endl;
    }

    static void debug(std::string text) {
        std::cout << "\033[32m" << PREFIX << " - [DEBUG]: " << text << std::endl;
    }
};

// Definition of the static member variable
const std::string Logger::PREFIX = "(bds-sdk-generator)";

#endif //LOGGER_H