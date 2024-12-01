#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 256
#define PROGRAM_SIZE 256

// Virtual Machine structure
typedef struct VM {
    int stack[STACK_SIZE];  // Stack for holding intermediate values
    int sp;                 // Stack pointer
    int ip;                 // Instruction pointer
    int program[PROGRAM_SIZE]; // Array to hold the program (bytecode)
} VM;

// Opcodes (Instruction set)
enum {
    HALT = 0,   // Stop execution
    PUSH,       // Push a value onto the stack
    ADD,        // Pop two values, add, push the result
    SUB,        // Pop two values, subtract, push the result
    MUL,        // Pop two values, multiply, push the result
    DIV,        // Pop two values, divide, push the result
    PRINT,      // Print the current value at the top of the stack
    MOD,        // Returns the remainder of two divided values, push the result
    NL,         // Prints a new line
};

// Push value onto the stack
void push(VM *vm, int value) {
    if (vm->sp < STACK_SIZE) {
        vm->stack[vm->sp++] = value;
    } else {
        printf("Stack overflow!\n");
        exit(1);
    }
}

// Pop value from the stack
int pop(VM *vm) {
    if (vm->sp > 0) {
        return vm->stack[--vm->sp];
    } else {
        printf("Stack underflow!\n");
        exit(1);
    }
}

void eval(VM *vm) {
    printf("Running VM... \n");
    int running = 1;
    while (running) {
        int opcode = vm->program[vm->ip++]; // Fetch instruction
        
        switch (opcode) {
            case HALT:  // Stop execution
                printf("Stopping execution...");
                running = 0;
                break;

            case PUSH: {  // Push a value onto the stack
                int value = vm->program[vm->ip++];  // Operand
                printf("Value pushed onto stack: %d\n", value);
                push(vm, value);
                break;
            }

            case ADD: {   // Add two values
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a + b: %i\n", a + b);
                push(vm, a + b);
                break;
            }

            case SUB: {   // Subtract two values
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a - b: %i\n", a - b);
                push(vm, a - b);
                break;
            }

            case MUL: {   // Multiply two values
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a * b: %d\n", a * b);
                push(vm, a * b);
                break;
            }

            case MOD: {
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a MOD b: %d\n", a % b);
                push(vm, a % b);
                break;
            }

            case DIV: {   // Divide two values
                int b = pop(vm);
                if (b == 0) {
                    printf("Error: Division by zero!\n");
                    exit(1);
                }
                int a = pop(vm);
                printf("Result after a / b: %d\n", a / b);
                push(vm, a / b);
                break;
            }

            case PRINT: {
                printf("%d\n", pop(vm));
                break;
            }

            case NL: {
                printf("\n");
                break;
            }

            default:
                printf("Unknown instruction %d\n", opcode);
                running = 0;
                break;
        }
    }
}

void load_program(VM *vm, int *program, int program_size) {
    printf("Loading program... \n");
    for (int i = 0; i < program_size; i++) {
        vm->program[i] = program[i];
    }
}

int main() {
    // Create a VM instance
    printf("Creating a VM instance... \n");
    VM vm = { .sp = 0, .ip = 0 };

    // Sample bytecode: PUSH, DIV ADD, PRINT, MOD, MUL, HALT
    int program[] = {
        PUSH, 100,
        PUSH, 4,
        DIV,
        PRINT, NL,

        PUSH, 10,
        PUSH, 20,
        ADD,
        PUSH, 3,
        MUL,
        PRINT, NL,

        PUSH, 50,
        PUSH, 6,
        MOD,
        PRINT, NL,

        HALT
    };

    // Load the program into the VM
    load_program(&vm, program, sizeof(program) / sizeof(int));

    // Run the program
    eval(&vm);

    return 0;
}
 