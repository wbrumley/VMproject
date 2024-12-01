#ifndef VM_H
#define VM_H

// **VM Configuration**
#define STACK_SIZE 256        // Size of the stack for intermediate values
#define PROGRAM_SIZE 256      // Size of the program memory to hold bytecode
#define NUM_VARIABLES 16      // Number of variables the VM can manage
#define NUM_FUNCTIONS 16      // Number of functions the VM can define
#define CALL_STACK_SIZE 16    // Size of the call stack for function return addresses

// **Opcodes (Instruction Set)**
// These define the operations that the VM can perform.
enum {
    HALT = 0,       // Stop execution
    PUSH,           // Push a value onto the stack
    ADD,            // Pop two values, add them, and push the result
    SUB,            // Pop two values, subtract them, and push the result
    MUL,            // Pop two values, multiply them, and push the result
    DIV,            // Pop two values, divide them, and push the result
    PRINT,          // Print the value at the top of the stack
    MOD,            // Pop two values, compute modulus, and push the result
    CMP,            // Compare two values on the stack and set a comparison flag
    IF,             // Execute the next instruction only if the comparison flag is true
    PRINT_EQUAL,    // Print "Equal" or "Not Equal" based on the comparison flag
    SET,            // Store a value from the stack into a variable
    GET,            // Retrieve a value from a variable and push it onto the stack
    FUNC,           // Define a function with a start address and parameter count
    CALL,           // Call a previously defined function
    RET,            // Return from a function to the caller
    NL              // Print a newline
};

// **Function Metadata**
// This structure is used to store metadata for each function defined in the VM.
typedef struct Function {
    int start_address;  // Address in the program memory where the function begins
    int num_params;     // Number of parameters the function expects
} Function;

// **Virtual Machine Structure**
// The `VM` structure represents the state of the virtual machine.
typedef struct VM {
    int stack[STACK_SIZE];         // Stack for holding intermediate values
    int sp;                        // Stack pointer, tracks the top of the stack
    int ip;                        // Instruction pointer, tracks the current instruction
    int program[PROGRAM_SIZE];     // Memory to hold the program's bytecode
    int cmp_flag;                  // Comparison flag: 1 if true, 0 if false
    int variables[NUM_VARIABLES];  // Array to store variable values
    Function functions[NUM_FUNCTIONS]; // Table to store metadata for defined functions
    int call_stack[CALL_STACK_SIZE];   // Call stack to store return addresses for function calls
    int call_sp;                   // Call stack pointer, tracks the top of the call stack
} VM;

// **Function Prototypes**
// These functions define the interface for the virtual machine.

// Push a value onto the stack
void push(VM *vm, int value);

// Pop a value from the stack
int pop(VM *vm);

// Execute the loaded bytecode
void eval(VM *vm);

// Load a program (bytecode) into the VM's memory
void load_program(VM *vm, int *program, int program_size);

#endif // VM_H
