#include <llvm-14/llvm/Demangle/Demangle.h>

#include <filesystem>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (const string fileName = "bds.pdb"; filesystem::exists(fileName)) {
        cout << fileName << " not found..." << endl;
        return EXIT_FAILURE;
    }

    auto syms =

    return EXIT_SUCCESS;
}
