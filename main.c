#include "vm.h" // Include the virtual machine header file to use the VM structure and related functionality

int main() {
    // Initialize the virtual machine
    printf("Creating a VM instance... \n");
    VM vm = { .sp = 0, .ip = 0, .cmp_flag = 0, .call_sp = 0 };

    // Define the bytecode program
    int program[] = {
        // Define a function to compare two numbers
        FUNC, 0, 2,    // Function[0] is defined, taking 2 parameters
        CMP,           // Compare the top two values on the stack
        PRINT_EQUAL,   // Print "Equal" or "Not Equal" based on the comparison result
        RET,           // Return from the function to the caller

        // Define a function to add two numbers
        FUNC, 1, 2,    // Function[1] is defined, taking 2 parameters
        ADD,           // Add the top two values on the stack
        RET,           // Return the result to the caller

        // Main program
        // Call Function[0] to compare 10 and 20
        PUSH, 10,      // Push 10 onto the stack (parameter 1 for the function)
        PUSH, 20,      // Push 20 onto the stack (parameter 2 for the function)
        CALL, 0,       // Call Function[0] to compare the two numbers
        NL,            // Print a newline for better readability

        // Call Function[1] to add 10 and 20
        PUSH, 10,      // Push 10 onto the stack (parameter 1 for the function)
        PUSH, 20,      // Push 20 onto the stack (parameter 2 for the function)
        CALL, 1,       // Call Function[1] to add the two numbers
        PRINT,         // Print the result of the addition (30)
        NL,            // Print a newline for better readability

        // Call Function[0] to compare 50 and 50
        PUSH, 50,      // Push 50 onto the stack (parameter 1 for the function)
        PUSH, 50,      // Push 50 onto the stack (parameter 2 for the function)
        CALL, 0,       // Call Function[0] to compare the two numbers
        NL,            // Print a newline for better readability

        HALT           // Halt the execution of the program
    };

    // Load the program into the virtual machine
    load_program(&vm, program, sizeof(program) / sizeof(int));

    // Start the execution of the program
    eval(&vm);

    return 0; // End the program
}
