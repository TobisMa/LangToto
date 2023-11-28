#pragma once
#ifndef H_ASSEMBLER
#define H_ASSEMBLER

#define FILE_OUT_PREFIX "out_"

#include "ast.h"

static const char* ASM_HEADER_DEFAULT[] = {
    "section .text",
    "  global _start",
    "  _start:",
    ""  // NOTE: required to mark end
};

int generateAssembly(const struct Program* programm, const char* fileName);
int fprintTemplate(FILE* file, const char* templateLines[]);

#endif
