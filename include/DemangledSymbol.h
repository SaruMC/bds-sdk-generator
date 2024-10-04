#pragma once

#ifndef DEMANGLEDSYMBOL_H
#define DEMANGLEDSYMBOL_H

#include <string>

struct DemangledSymbol {
  std::string value;
  uint32_t rva; // relative virtual address
  uint32_t offset;
};

#endif //DEMANGLEDSYMBOL_H