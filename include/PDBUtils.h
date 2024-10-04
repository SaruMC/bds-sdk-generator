#pragma once

#ifndef PDBUTILS_H
#define PDBUTILS_H

#include "MemoryMappedFile.h"
#include "DemangledSymbol.h"

#include <llvm/DebugInfo/PDB/Raw/PDBFile.h>
#include <llvm/DebugInfo/PDB/Raw/RawFile.h>
#include <llvm/DebugInfo/PDB/Raw/DBIStream.h>

#include <vector>

class PDBUtils {
private:
    std::vector<DemangledSymbol*> symbols;
    bool hasValidDBIStreams(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream);

public:
    std::vector<DemangledSymbol*>* loadPdb();
    void getClassFromSymbol(const std::string& className, std::vector<DemangledSymbol*>* symbols);
};

#endif //PDBUTILS_H