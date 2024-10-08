#include <llvm/Demangle/Demangle.h>

#include "Logger.h"
#include "PDBUtils.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <regex>

namespace fs = std::filesystem;
using namespace std;

std::string sanitizeClassName(const std::string& className) {
    std::string sanitized = className;
    std::regex special_chars("[^a-zA-Z0-9]");
    sanitized = std::regex_replace(sanitized, special_chars, "_");
    return sanitized;
}

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return EXIT_FAILURE;
  }

  const std::string pdbFileName = argv[1];
  const std::string className = "Actor"; // or pass this as an additional argument for flexibility
  const std::string headerFileName = sanitizeClassName(className) + "Symbols.h";

  if (!fs::exists(pdbFileName) || pdbFileName.size() < 5 || pdbFileName.substr(pdbFileName.size() - 4) != ".pdb") {
    Logger::error("The given file either does not exist or is not a .pdb file");
    return EXIT_FAILURE;
  }

  Logger::info("Loading symbols from given pdb file...");
  PDBUtils pdbUtils;
  const auto symbols = pdbUtils.loadPdb(pdbFileName.c_str());

  if (!symbols) {
    Logger::error("Failed to load symbols from the pdb file");
    return EXIT_FAILURE;
  }

  Logger::info("Symbols loaded successfully!");

  std::ofstream headerFile(headerFileName);
  if (!headerFile.is_open()) {
    Logger::error("Failed to open the output header file");
    delete symbols;
    return EXIT_FAILURE;
  }

  headerFile << "#ifndef " << sanitizeClassName(className) << "_SYMBOLS_H\n";
  headerFile << "#define " << sanitizeClassName(className) << "_SYMBOLS_H\n\n";
  headerFile << "#pragma once\n\n";
  headerFile << "#include <string>\n";
  headerFile << "#include <vector>\n";
  headerFile << "#include <iostream>\n\n";
  headerFile << "class " << sanitizeClassName(className) <<"Symbols {\n";
  headerFile << "public:\n";
  headerFile << "    static const std::vector<std::string> symbols;\n\n";
  headerFile << "    static void printSymbols() {\n";
  headerFile << "        for (const auto& symbol : symbols) {\n";
  headerFile << "            std::cout << symbol << std::endl;\n";
  headerFile << "        }\n";
  headerFile << "    }\n";
  headerFile << "};\n\n";
  headerFile << "const std::vector<std::string> " << sanitizeClassName(className) << "Symbols::symbols = {\n";

  const std::vector<std::string>* classSymbols = PDBUtils::getClassFromSymbol(className, symbols);

  for (const auto& symbol : *classSymbols) {
    headerFile << "    \"" << symbol << "\",\n";
  }

  headerFile << "};\n\n";
  headerFile << "#endif // " << sanitizeClassName(className) << "_SYMBOLS_H\n";

  headerFile.close();
  Logger::info("Symbols written to header file successfully!");

  delete symbols;
  free(symbols);

  return EXIT_SUCCESS;
}