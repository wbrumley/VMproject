#include "vm.h" // Include the VM structure and opcode definitions
#include <stdio.h>
#include <stdlib.h>

// Predefined string table for PRINT_EQUAL
// Used to output comparison results ("Equal" or "Not Equal")
const char *string_table[] = {
    "Equal",
    "Not Equal"
};

// Push a value onto the VM stack
void push(VM *vm, int value) {
    if (vm->sp < STACK_SIZE) { // Ensure stack does not overflow
        vm->stack[vm->sp++] = value; // Store value and increment stack pointer
    } else {
        printf("Stack overflow!\n");
        exit(1); // Exit on stack overflow
    }
}

// Pop a value from the VM stack
int pop(VM *vm) {
    if (vm->sp > 0) { // Ensure stack is not empty
        return vm->stack[--vm->sp]; // Decrement stack pointer and return value
    } else {
        printf("Stack underflow!\n");
        exit(1); // Exit on stack underflow
    }
}

// Main function to execute the bytecode loaded into the VM
void eval(VM *vm) {
    printf("Running VM... \n");
    int running = 1; // Flag to indicate if the VM should continue execution

    while (running) {
        int opcode = vm->program[vm->ip++]; // Fetch the next instruction

        // Process the current instruction
        switch (opcode) {
            case HALT:
                printf("Stopping execution...\n");
                running = 0; // Stop the VM
                break;

            case PUSH: { // Push a constant onto the stack
                int value = vm->program[vm->ip++]; // Fetch the value
                printf("Value pushed onto stack: %d\n", value);
                push(vm, value);
                break;
            }

            case ADD: { // Add the top two values on the stack
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a + b: %d\n", a + b);
                push(vm, a + b); // Push the result back onto the stack
                break;
            }

            case SUB: { // Subtract the top two values on the stack
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a - b: %d\n", a - b);
                push(vm, a - b); // Push the result back onto the stack
                break;
            }

            case MUL: { // Multiply the top two values on the stack
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a * b: %d\n", a * b);
                push(vm, a * b); // Push the result back onto the stack
                break;
            }

            case DIV: { // Divide the top two values on the stack
                int b = pop(vm);
                if (b == 0) { // Prevent division by zero
                    printf("Error: Division by zero!\n");
                    exit(1);
                }
                int a = pop(vm);
                printf("Result after a / b: %d\n", a / b);
                push(vm, a / b); // Push the result back onto the stack
                break;
            }

            case MOD: { // Compute modulus of the top two values
                int b = pop(vm);
                int a = pop(vm);
                printf("Result after a MOD b: %d\n", a % b);
                push(vm, a % b); // Push the result back onto the stack
                break;
            }

            case PRINT: // Print the value at the top of the stack
                printf("%d\n", pop(vm));
                break;

            case NL: // Print a newline
                printf("\n");
                break;

            case CMP: { // Compare the top two values on the stack
                int b = pop(vm);
                int a = pop(vm);
                vm->cmp_flag = (a == b); // Set the comparison flag
                printf("Comparison: %d == %d -> %d\n", a, b, vm->cmp_flag);
                break;
            }

            case IF: { // Conditional execution based on cmp_flag
                if (!vm->cmp_flag) { // If false, skip the next instruction
                    printf("IF condition failed. Skipping next instruction...\n");
                    vm->ip++;
                } else {
                    printf("IF condition passed. Continuing execution...\n");
                }
                break;
            }

            case PRINT_EQUAL: { // Print "Equal" or "Not Equal" based on cmp_flag
                printf("%s\n", string_table[vm->cmp_flag ? 0 : 1]);
                break;
            }

            case SET: { // Store a value in a variable
                int var_index = vm->program[vm->ip++]; // Fetch variable index
                if (var_index >= 0 && var_index < NUM_VARIABLES) {
                    vm->variables[var_index] = pop(vm); // Pop value and store
                    printf("Variable[%d] set to %d\n", var_index, vm->variables[var_index]);
                } else {
                    printf("Error: Invalid variable index %d\n", var_index);
                    exit(1);
                }
                break;
            }

            case GET: { // Retrieve a value from a variable
                int var_index = vm->program[vm->ip++]; // Fetch variable index
                if (var_index >= 0 && var_index < NUM_VARIABLES) {
                    push(vm, vm->variables[var_index]); // Push value onto the stack
                    printf("Variable[%d] retrieved: %d\n", var_index, vm->variables[var_index]);
                } else {
                    printf("Error: Invalid variable index %d\n", var_index);
                    exit(1);
                }
                break;
            }

            case FUNC: { // Define a function
                int func_index = vm->program[vm->ip++];
                int num_params = vm->program[vm->ip++];
                if (func_index >= 0 && func_index < NUM_FUNCTIONS) {
                    // Store function metadata
                    vm->functions[func_index].start_address = vm->ip;
                    vm->functions[func_index].num_params = num_params;
                    printf("Function[%d] defined with %d parameter(s) at address %d\n",
                           func_index, num_params, vm->ip);

                    // Skip over the function body until RET is encountered
                    while (vm->program[vm->ip] != RET) {
                        vm->ip++;
                        if (vm->ip >= PROGRAM_SIZE) {
                            printf("Error: Missing RET for function[%d]\n", func_index);
                            exit(1);
                        }
                    }
                    vm->ip++; // Skip the RET opcode
                } else {
                    printf("Error: Invalid function index %d\n", func_index);
                    exit(1);
                }
                break;
            }

            case CALL: { // Call a function
                int func_index = vm->program[vm->ip++];
                if (func_index >= 0 && func_index < NUM_FUNCTIONS) {
                    Function *func = &vm->functions[func_index];
                    vm->call_stack[vm->call_sp++] = vm->ip; // Push return address
                    vm->ip = func->start_address;          // Jump to function start
                    printf("Calling Function[%d] with %d parameter(s)\n",
                           func_index, func->num_params);
                } else {
                    printf("Error: Invalid function index %d\n", func_index);
                    exit(1);
                }
                break;
            }

            case RET: // Return from a function
                vm->ip = vm->call_stack[--vm->call_sp]; // Pop return address
                printf("Returning from function\n");
                break;

            default: // Handle unknown opcodes
                printf("Unknown instruction %d\n", opcode);
                running = 0;
                break;
        }
    }
}

// Load a program into the VM's memory
void load_program(VM *vm, int *program, int program_size) {
    printf("Loading program... \n");
    for (int i = 0; i < program_size; i++) {
        vm->program[i] = program[i];
    }
}
