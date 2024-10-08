#include "Logger.h"

const std::string Logger::PREFIX = "(bds-sdk-generator)";

const std::string Logger::COLOR_RED = "\x1b[31m";
const std::string Logger::COLOR_GREEN = "\x1b[32m";
const std::string Logger::COLOR_YELLOW = "\x1b[33m";
const std::string Logger::COLOR_RESET = "\033[0m";

void Logger::info(const std::string& text) {
  std::cout << COLOR_YELLOW << PREFIX << " - [INFO]: " << text << COLOR_RESET << std::endl;
}

void Logger::error(const std::string& text) {
  std::cerr << COLOR_RED << PREFIX << " - [ERROR]: " << text << COLOR_RESET << std::endl;
}

void Logger::debug(const std::string& text) {
  std::cout << COLOR_GREEN << PREFIX << " - [DEBUG]: " << text <<  COLOR_RESET << std::endl;
}