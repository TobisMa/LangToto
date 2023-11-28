#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "save_stdlib.h"
#include "ast.h"
#include "assembler.h"

void printIncorrectUsage(const char *executablePath)
{
    printf("Incorrect usage\nHow to use: %s <toto_file_path>\n", executablePath);
}

void printTooManyFiles(const char *executablePath)
{
    printf("You can't give multiple files as input (yet) ");
    printIncorrectUsage(executablePath);
}

void printErrnoFail(char *funcName)
{
    printf("%s() failed with errno %i", funcName, errno);
}

long fileSize(FILE *file)
{
    if (!file)
    {
        return -1;
    }
    fpos_t originalPos;
    if (fgetpos(file, &originalPos) != 0)
    {
        printErrnoFail("fgetpos");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long endPos = ftell(file);
    if (fsetpos(file, &originalPos) != 0)
    {
        printErrnoFail("fsetpos");
        return -1;
    }
    return endPos;
}

char *readWholeFile(const char *fileName, long *o_size)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Failed to open file '%s'\n", fileName);
        fclose(file);
        return NULL;
    }
    long size = fileSize(file);
    if (size == -1)
    {
        fclose(file);
        return NULL;
    }
    *o_size = size;
    char *fileContents = saveMalloc(sizeof(char) * (size + 1));
    fread(fileContents, 1, size, file);
    fclose(file);
    fileContents[size] = '\0';
    return fileContents;
}

int main(int argc, char const *argv[])
{
    if (argc <= 1)
    {
        printIncorrectUsage(argv[0]);
        return 1;
    }
    else if (argc >= 3)
    {
        printTooManyFiles(argv[0]);
        return 1;
    }

    long size;
    char *fileContents = readWholeFile(argv[1], &size);
    if (fileContents == NULL)
    {
        return 1;
    }
    printf("File:\n====START=====\n%s=====END======\n\nSize: %ld\n", fileContents, size);

    int stmtCount = 0;
    int* p_stmtCount = &stmtCount;
    struct TokenListNode *tokens = parse(fileContents, size, p_stmtCount);
    free(fileContents);
    if (tokens == NULL) {
        printf("Failed creating a token list\n");
        return -1;
    }

    printf("Found statements: %i\n", stmtCount);

    struct Program* astTree = createAST(tokens, stmtCount);
    if (astTree == NULL) {
        printf("Failed generating AST Tree\n");
        freeTokenList(tokens);
        return -1;
    }
    freeTokenList(tokens);

    // for (int i = 0; i < astTree->count; i++) {
    //     printf("P%p; ", &astTree->statements[i]);
    //     if (astTree->statements[i].stmt_type == AST_STMT_EXIT) {
    //         printf("Exit code: %s\n", astTree->statements[i].exitStmt.exit_code);
    //     }
    // }

    int result = generateAssembly(astTree, "totoOut.asm");
    freeAstTree(astTree);
    if (result != 0) {
        printf("Failure generating assembly: %i\n", result);
    } 


    // don't wanna type it all the time:
    system("nasm -felf64 totoOut.asm");
    system("ld totoOut.o -o totoExec");

    return 0;
}
