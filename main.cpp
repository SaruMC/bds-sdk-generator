#include <llvm/Demangle/Demangle.h>

#include "Logger.h"
#include "PDBUtils.h"

#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using namespace std;

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return EXIT_FAILURE;
  }

  const std::string fileName = argv[1];

  if (!fs::exists(fileName) || fileName.size() < 5 || fileName.substr(fileName.size() - 4) != ".pdb") {
    Logger::error("The given file either does not exist or is not a .pdb file");
    return EXIT_FAILURE;
  }

  Logger::info("Loading symbols from given pdb file...");
  PDBUtils pdbUtils;
  const auto symbols = pdbUtils.loadPdb(fileName.c_str());

  if (!symbols) {
    Logger::error("Failed to load symbols from the pdb file");
    return EXIT_FAILURE;
  }

  Logger::info("Symbols loaded successfully!");

  pdbUtils.getClassFromSymbol("Actor", symbols);
  delete symbols;

  return EXIT_SUCCESS;
}
