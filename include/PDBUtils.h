#pragma once

#ifndef PDBUTILS_H
#define PDBUTILS_H

#include "DemangledSymbol.h"

#include <vector>
#include <string>
#include <llvm/DebugInfo/PDB/IPDBSession.h>
#include <llvm/DebugInfo/PDB/GenericError.h>

class PDBUtils {
private:
    std::vector<DemangledSymbol*> symbols;
    bool hasValidDBIStreams(const llvm::pdb::IPDBSession& session, const llvm::pdb::PDBSymbolExe& exe);

public:
    std::vector<DemangledSymbol*>* loadPdb();
    void getClassFromSymbol(const std::string& className, std::vector<DemangledSymbol*>* symbols);
};

#endif //PDBUTILS_H