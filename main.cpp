#include <llvm-14/llvm/Demangle/Demangle.h>

#include "Logger.h"

#include <filesystem>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (const string fileName = "bds.pdb"; filesystem::exists(fileName)) {
        Logger::error("bds.pdb does not exist");
        return EXIT_FAILURE;
    }

    Logger::info("Loading symbols from bds.pdb");
    auto syms = "";

    return EXIT_SUCCESS;
}
