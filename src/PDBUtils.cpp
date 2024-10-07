#include "PDBUtils.h"
#include "MemoryMappedFile.h"
#include "DemangledSymbol.h"
#include "Logger.h"

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

using namespace std;
using namespace llvm::pdb;

namespace {
    bool IsError(pdb_error_code error_code) {
        switch (error_code) {
            case pdb_error_code::Success:
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

    bool HasValidDBIStreams(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream) {
        if (IsError(dbiStream.HasValidImageSectionStream(rawPdbFile))) return false;
        if (IsError(dbiStream.HasValidPublicSymbolStream(rawPdbFile))) return false;
        if (IsError(dbiStream.HasValidGlobalSymbolStream(rawPdbFile))) return false;
        if (IsError(dbiStream.HasValidSectionContributionStream(rawPdbFile))) return false;
        return true;
    }
}

vector<DemangledSymbol*>* PDBUtils::parsePdb(const PDB::RawFile& rawPdbFile, const PDB::DBIStream& dbiStream) {
    symbols = new vector<DemangledSymbol*>;

    const PDB::ImageSectionStream imageSectionStream = dbiStream.CreateImageSectionStream(rawPdbFile);
    const PDB::CoalescedMSFStream symbolRecordStream = dbiStream.CreateSymbolRecordStream(rawPdbFile);
    const PDB::PublicSymbolStream publicSymbolStream = dbiStream.CreatePublicSymbolStream(rawPdbFile);

    const PDB::ArrayView<PDB::HashRecord> hashRecords = publicSymbolStream.GetRecords();
    const size_t count = hashRecords.GetLength();

    for (const PDB::HashRecord& hashRecord : hashRecords) {
        const PDB::CodeView::DBI::Record* record = publicSymbolStream.GetRecord(symbolRecordStream, hashRecord);
        const uint32_t rva = imageSectionStream.ConvertSectionOffsetToRVA(record->data.S_PUB32.section, record->data.S_PUB32.offset);

        if (rva == 0u) continue;

        auto sym = new DemangledSymbol;
        sym->value = Demangler::Demangle(record->data.S_PUB32.name);
        sym->rva = rva;
        sym->offset = record->data.S_PUB32.offset;

        symbols->push_back(sym);
    }

    return symbols;
}

vector<DemangledSymbol*>* PDBUtils::loadPdb() {
    const char* const pdbPath = "./bedrock_server.pdb";
    MemoryMappedFileHandle pdbFile = MemoryMappedFile::Open(pdbPath, "r");

    if (!pdbFile.baseAddress) {
        Logger::info("[PDB] PDB file not found");
        return nullptr;
    }

    if (IsError(PDB::ValidateFile(pdbFile.baseAddress))) {
        MemoryMappedFile::Close(pdbFile);
        return nullptr;
    }

    const PDB::RawFile rawPdbFile = PDB::CreateRawFile(pdbFile.baseAddress);

    if (IsError(PDB::HasValidDBIStream(rawPdbFile))) {
        MemoryMappedFile::Close(pdbFile);
        return nullptr;
    }

    const PDB::InfoStream infoStream(rawPdbFile);

    if (infoStream.UsesDebugFastLink()) {
        MemoryMappedFile::Close(pdbFile);
        return nullptr;
    }

    const PDB::DBIStream dbiStream = PDB::CreateDBIStream(rawPdbFile);

    if (!HasValidDBIStreams(rawPdbFile, dbiStream)) {
        MemoryMappedFile::Close(pdbFile);
        return nullptr;
    }

    return parsePdb(rawPdbFile, dbiStream);
}