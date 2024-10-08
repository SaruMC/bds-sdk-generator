#ifndef PDBUTILS_H
#define PDBUTILS_H

#pragma once

#include <vector>
#include "DemangledSymbol.h"
#include "PDB/PDB_RawFile.h"
#include "PDB/PDB_DBIStream.h"

class PDBUtils {
private:
    std::vector<DemangledSymbol*> symbols;
    bool hasValidDBIStreams(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream);

public:
    ~PDBUtils();  // Destructor declaration

    std::vector<DemangledSymbol*>* loadPdb(const char* fileName);
    static std::vector<std::string>* getClassFromSymbol(const std::string& className, std::vector<DemangledSymbol*>* symbols);
    std::vector<DemangledSymbol*>* parsePdb(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream);  // Added parsePdb declaration
};

#endif // PDBUTILS_H