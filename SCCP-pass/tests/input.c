//=============================================================================
// FILE:
//      input.c
//
// DESCRIPTION:
//      Sample input file
//
// License: MIT
//=============================================================================

// int req1() {
//     int d;
//     if (1) {
//         int a = 5;
//         int b = 5;

//         int c = a + b - 2;
//         d = c * 2 + 3;
//     }

//     return d * 4;
// }

// int req2() {
//     int d;
//     if (0) {
//         int a = 5;
//         int b = 5;

//         int c = a + b - 2;
//         d = c * 2 + 3;
//     }

//     if (1) {
//         d = 1;
//         d = 2;
//         d = 3;
//         d = 4;
//         d = 5;
//     }

//     return d * 4;
// }

// Test 10: Constants through pointers
int test_pointer_constants() {
    int x = 42;
    int *ptr = &x;
    return *ptr;  // Should propagate 42
}

// Test 11: Array access with constant indices
int test_array_constants() {
    int arr[5] = {1, 2, 3, 4, 5};
    return arr[2];  // Should be 3
}

// Test 12: Mixed constant and variable propagation
int test_mixed_propagation(int input) {
    const int base = 100;
    int variable = input;
    int result = base + variable; // base is constant, variable is not

    if (input > 50) { // Always true
        result += 10;
    }

    return result;
}

// Test 13: Partially constant expressions
int test_partial_constants(int dynamic_val) {
    const int fixed = 8;
    int result = fixed * 2; // Should be 16

    if (dynamic_val > 0) {
        result += dynamic_val; // Cannot be constant propagated
    }

    return result;
}

// Test 14: Dead code elimination with constants
int test_dead_code() {
    int x = 5;
    if (0) {  // Always false
        x = 10;  // Dead code
    }
    return x;  // Should be 5
}

// Test 15: Constant propagation through multiple assignments
int test_multiple_assignments() {
    int a = 1;
    int b = a;     // b = 1
    int c = b + 2; // c = 3
    int d = c * a; // d = 3
    
    return d;      // Should be 3
}