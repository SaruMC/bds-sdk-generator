#pragma once

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <list>
#include <string_view>
#include <thread>
#include <vector>
#include <mutex>
#include <iomanip>
#include <memory>

#include "PDB/PDB.h"
#include "PDB/PDB_RawFile.h"
#include "PDB/PDB_DBIStream.h"
#include "PDB/PDB_InfoStream.h"
#include "PDB/PDB_ErrorCodes.h"
#include "PDB/PDB_InfoStream.h"
#include "PDB/Foundation/PDB_DisableWarningsPop.h"

#include <llvm/Demangle/Demangle.h>

#include "Logger.h"
#include "DemangledSymbol.h"
#include "MemoryMappedFile.h"

using std::string, std::vector;

namespace
{
    PDB_NO_DISCARD static bool IsError(PDB::ErrorCode errorCode)
    {
        switch (errorCode)
        {
        case PDB::ErrorCode::Success:
            return false;

        case PDB::ErrorCode::InvalidSuperBlock:
            Logger::error("Invalid Superblock");
            return true;

        case PDB::ErrorCode::InvalidFreeBlockMap:
            Logger::error("Invalid free block map");
            return true;

        case PDB::ErrorCode::InvalidSignature:
            Logger::error("Invalid stream signature");
            return true;

        case PDB::ErrorCode::InvalidStreamIndex:
            Logger::error("Invalid stream index");
            return true;

        case PDB::ErrorCode::UnknownVersion:
            Logger::error("Unknown version");
            return true;
        }
        return true;
    }

    PDB_NO_DISCARD static bool HasValidDBIStreams(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream)
    {
        if (IsError(dbiStream.HasValidImageSectionStream(rawPdbFile))) return false;
        if (IsError(dbiStream.HasValidPublicSymbolStream(rawPdbFile))) return false;
        if (IsError(dbiStream.HasValidGlobalSymbolStream(rawPdbFile))) return false;
        if (IsError(dbiStream.HasValidSectionContributionStream(rawPdbFile))) return false;
        return true;
    }
} // namespace

class PDBUtils {
private:
    std::unique_ptr<vector<DemangledSymbol*>> symbols = std::make_unique<vector<DemangledSymbol*>>();

public:
    ~PDBUtils()
    {
        for (auto symbol : *symbols)
        {
            delete symbol;
        }
    }

    void getClassFromSymbol(std::string className, std::vector<DemangledSymbol*>* symbols)
    {
        for (auto symbol : *symbols)
        {
            // Convention calling
            if (symbol->value.find("__cdecl " + className + "::") != std::string::npos) {
                // ToDo
                Logger::debug(std::to_string(symbol->offset));
            }
        }
    }

    vector<DemangledSymbol*>* parsePdb(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream)
    {
        symbols = std::make_unique<vector<DemangledSymbol*>>();

        const PDB::ImageSectionStream imageSectionStream = dbiStream.CreateImageSectionStream(rawPdbFile);
        const PDB::CoalescedMSFStream symbolRecordStream = dbiStream.CreateSymbolRecordStream(rawPdbFile);
        const PDB::PublicSymbolStream publicSymbolStream = dbiStream.CreatePublicSymbolStream(rawPdbFile);
        {
            const PDB::ArrayView<PDB::HashRecord> hashRecords = publicSymbolStream.GetRecords();
            const size_t count = hashRecords.GetLength();

            for (const PDB::HashRecord& hashRecord : hashRecords)
            {
                const PDB::CodeView::DBI::Record* record = publicSymbolStream.GetRecord(symbolRecordStream, hashRecord);
                const uint32_t rva = imageSectionStream.ConvertSectionOffsetToRVA(record->data.S_PUB32.section, record->data.S_PUB32.offset);

                if (rva == 0u)
                    continue;

                auto sym = new DemangledSymbol;
                sym->value = llvm::demangle(record->data.S_PUB32.name);
                sym->rva = rva;
                sym->offset = record->data.S_PUB32.offset;

                symbols->push_back(sym);
            }
        }

        return symbols.get();
    }

    vector<DemangledSymbol*>* loadPdb()
    {
        const wchar_t* const pdbPath = LR"(./bedrock_server.pdb)";
        MemoryMappedFileHandle pdbFile = MemoryMappedFile::Open(pdbPath);

        if (!pdbFile.baseAddress)
        {
            Logger::info("[PDB] PDB file not found");
            return nullptr;
        }

        if (IsError(PDB::ValidateFile(pdbFile.baseAddress)))
        {
            MemoryMappedFile::Close(pdbFile);
            return nullptr;
        }

        const PDB::RawFile rawPdbFile = PDB::CreateRawFile(pdbFile.baseAddress);

        if (IsError(PDB::HasValidDBIStream(rawPdbFile)))
        {
            MemoryMappedFile::Close(pdbFile);
            return nullptr;
        }

        const PDB::InfoStream infoStream(rawPdbFile);

        if (infoStream.UsesDebugFastLink())
        {
            MemoryMappedFile::Close(pdbFile);
            return nullptr;
        }

        const PDB::DBIStream dbiStream = PDB::CreateDBIStream(rawPdbFile);

        if (!HasValidDBIStreams(rawPdbFile, dbiStream))
        {
            MemoryMappedFile::Close(pdbFile);
            return nullptr;
        }

        return parsePdb(rawPdbFile, dbiStream);
    }
};

extern PDBUtils pdbUtils;