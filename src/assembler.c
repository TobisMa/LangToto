#include "errno.h"
#include "assembler.h"


int generateAssembly(const struct Program* program, const char* fileName) {

    printf("Generating Assembly\n");
    FILE* f_out = fopen(fileName, "w");

    int ok = fprintTemplate(f_out, ASM_HEADER_DEFAULT);
    if (ok != 0) {
        printf("Failure writing template with exit code %i\n", ok);
        return -1;
    }

    for (int i = 0; i < program->count; i++) {
        switch (program->statements[i].stmt_type) {
            case AST_STMT_EXIT:
                fprintf(f_out, "    mov rax, 60\n");
                fprintf(f_out, "    mov rdi, %s\n", program->statements[i].exitStmt.exit_code);
                fprintf(f_out, "    syscall\n\n");
                break;
            
            default:
                printf("Unsupported statement (for now): %i\n", program->statements[i].stmt_type);
        }
    }

    fclose(f_out);

    printf("Successfully generated Assembly into file '%s'\n", fileName);
    return 0;

}


int fprintTemplate(FILE* file, const char* templateLines[]) {
    int i = 0;
    while (templateLines[i][0] != '\0') {
        int ok = fprintf(file, "%s\n", templateLines[i]);
        if (ok < 0) {
            printf("fprintf failed with code %i and errni %i\n", ok, errno);
            return -1;
        }
        printf("Wrote '%s'\n", templateLines[i]);
        i++;
    }
    printf("\n");
    return 0;
}
