#include <llvm/Demangle/Demangle.h>

#include "Logger.h"

#include <filesystem>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
     if (argc != 2) {
         cerr << "Usage: " << argv[0] << " <filename>" << endl;
         return EXIT_FAILURE;
     }

    string fileName = argv[1];

    if (!filesystem::exists(fileName) || fileName.size() <= 4 || fileName.substr(fileName.size() - 4) != ".pdb") {
        Logger::error("The given file is either not existsing or not a .pdb file");
        return EXIT_FAILURE;
    }

    Logger::info("Loading symbols from given pdb file...");
    // auto syms = loadPDBFile(fileName);

    return EXIT_SUCCESS;
}
