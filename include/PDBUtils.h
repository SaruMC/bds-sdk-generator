#pragma once

#ifndef PDBUTILS_H
#define PDBUTILS_H

#include "DemangledSymbol.h"
#include "PDB/PDB_RawFile.h"
#include "PDB/PDB_ErrorCodes.h"
#include "PDB/PDB_DBIStream.h"
#include "PDB/PDB_InfoStream.h"

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