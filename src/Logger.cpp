#include "Logger.h"

// Definition of the static member variable
const std::string Logger::PREFIX = "(bds-sdk-generator)";

void Logger::info(const std::string& text) {
  std::cout << "\033[33m" << PREFIX << " - [INFO]: " << text << std::endl;
}

void Logger::error(const std::string& text) {
  std::cerr << "\033[31m" << PREFIX << " - [ERROR]: " << text << std::endl;
}

void Logger::debug(const std::string& text) {
  std::cout << "\033[32m" << PREFIX << " - [DEBUG]: " << text << std::endl;
}