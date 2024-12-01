#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define opcodes
#define PUSH      1
#define ADD       2
#define SUB       3
#define MUL       4
#define DIV       5
#define MOD       6
#define PRINT     7
#define PRINT_STR 8
#define CMP       9
#define IF        10
#define JUMP      11
#define HALT      12

// Stack for the VM
#define STACK_SIZE 256
int stack[STACK_SIZE];
int sp = -1;  // Stack pointer

// Comparison flag
int cmp_flag = 0;  // 1 if comparison is true, 0 otherwise

// String table
const char* string_table[] = {
    "Equal",
    "Not Equal"
};

// Bytecode interpreter function
void execute(int *bytecode, int bytecode_size) {
    int ip = 0;  // Instruction pointer

    while (ip < bytecode_size) {
        int instruction = bytecode[ip];

        switch (instruction) {
            case PUSH: {
                int value = bytecode[++ip];
                stack[++sp] = value;
                break;
            }
            case ADD: {
                int b = stack[sp--];
                int a = stack[sp--];
                stack[++sp] = a + b;
                break;
            }
            case SUB: {
                int b = stack[sp--];
                int a = stack[sp--];
                stack[++sp] = a - b;
                break;
            }
            case MUL: {
                int b = stack[sp--];
                int a = stack[sp--];
                stack[++sp] = a * b;
                break;
            }
            case DIV: {
                int b = stack[sp--];
                int a = stack[sp--];
                if (b == 0) {
                    printf("Division by zero error\n");
                    exit(1);
                }
                stack[++sp] = a / b;
                break;
            }
            case MOD: {
                int b = stack[sp--];
                int a = stack[sp--];
                stack[++sp] = a % b;
                break;
            }
            case PRINT: {
                int value = stack[sp--];
                printf("%d\n", value);
                break;
            }
            case PRINT_STR: {
                int index = bytecode[++ip];
                printf("%s\n", string_table[index]);
                break;
            }
            case CMP: {
                int b = stack[sp--];
                int a = stack[sp--];
                cmp_flag = (a == b);
                break;
            }
            case IF: {
                if (!cmp_flag) {
                    // Skip the next instruction if the comparison is false
                    ip++;
                }
                break;
            }
            case JUMP: {
                int address = bytecode[++ip];
                ip = address - 1;  // Jump to the specified address
                break;
            }
            case HALT: {
                return;
            }
            default: {
                printf("Unknown instruction: %d\n", instruction);
                exit(1);
            }
        }
        ip++;
    }
}

int main() {
    // Example bytecode that includes division, modulus, comparison, and jumps
    int bytecode[] = {
        PUSH, 100,
        PUSH, 4,
        DIV,
        PRINT,

        PUSH, 25,
        PUSH, 25,
        CMP,
        IF,
        PRINT_STR, 0,  // Print "Equal"

        PUSH, 10,
        PUSH, 3,
        MOD,
        PRINT,

        HALT
    };

    int bytecode_size = sizeof(bytecode) / sizeof(int);
    execute(bytecode, bytecode_size);
    return 0;
}
