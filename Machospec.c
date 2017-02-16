#include "Machospec.h"

#include <mach-o/nlist.h>
#include <stdio.h>
#include <stdlib.h>

const char **machospec_copy_image_paths(struct dyld_all_image_infos* infos, uint32_t *count) {
  uint32_t cnt = infos->infoArrayCount;
  *count = cnt;
  const char **result = (const char**)malloc(sizeof(char*) * cnt);
  for (uint32_t i = 0; i < cnt; i++) {
    result[i] = infos->infoArray[i].imageFilePath;
  }
  return result;
}

void **machospec_copy_local_symbols(struct mach_header_64* header,
                                    const char ***symbolNames,
                                    uint32_t *count) {
  struct load_command* cmd = (struct load_command *)((char *)header + sizeof(struct mach_header_64));
  struct load_command* end = (struct load_command *)((char *)header + header->sizeofcmds);
  struct symtab_command* symtabCommand = NULL;
  for (; cmd < end; cmd = (struct load_command *)((char *)cmd + cmd->cmdsize)) {
    if (cmd->cmd == LC_SYMTAB) {
      symtabCommand = (struct symtab_command *)cmd;
      break;
    }
  }
  if (!symtabCommand) {
    return NULL;
  }
  struct nlist_64* symbolTable = (struct nlist_64 *)((char *)header + symtabCommand->symoff);
  printf("Got header: %p symbolTable: %p offset %u\n", header, symbolTable, symtabCommand->symoff);
  uint32_t symbolTableCount = symtabCommand->nsyms;
  printf("Got %u symbols\n", symbolTableCount);
  const char *stringTable = (const char *)header + symtabCommand->stroff;
  printf("Got stroff: %u %p\n", symtabCommand->stroff, stringTable);
  //char *stringTableEnd = stringTable + symtabCommand->strsize;
  const char **localSymbolNames = (const char **)malloc(sizeof(char *) * symbolTableCount);
  void **symbols = (void **)malloc(sizeof(void *) * symbolTableCount);
  uint32_t localCount = 0;
  for (uint32_t i = 0; i < symbolTableCount; i++) {
    if (symbolTable[i].n_type & N_STAB) {
      printf("Found bad symbol: %s %s %s\n", stringTable + symbolTable[i].n_un.n_strx, (symbolTable[i].n_type & N_STAB) ? "stab" : "", (symbolTable[i].n_type & N_EXT) ? "external" : "");
      continue;
    }
    if ((symbolTable[i].n_type & N_SECT) == 0) {
      printf("Found bad symbol: %s No section found\n", stringTable + symbolTable[i].n_un.n_strx);
      continue;
    }
    localCount += 1;
    localSymbolNames[i] = stringTable + symbolTable[i].n_un.n_strx;
    symbols[i] = (void *)((char *)header + symbolTable[i].n_value);
  }
  localSymbolNames = reallocf(localSymbolNames, sizeof(char *) * localCount);
  symbols = reallocf(symbols, sizeof(void *) * localCount);
  *count = localCount;
  *symbolNames = localSymbolNames;
  return symbols;
}

