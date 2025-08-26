//=============================================================================
// FILE:
//      input.c
//
// DESCRIPTION:
//      Sample input file
//
// License: MIT
//=============================================================================
// int
// foo(int a) {
//   return a * 2;
// }

// int
// bar(int a, int b) {
//   return (a + foo(b) * 2);
// }

// int
// fez(int a, int b, int c) {
//   return (a + bar(a, b) * 2 + c * 3);
// }

// int
// main(int argc, char *argv[]) {
//   int a = 123;
//   int ret = 0;

//   ret += foo(a);
//   ret += bar(a, ret);
//   ret += fez(a, ret, 123);

//   return ret;
// }

int req1() {
    int d;
    if (0) {
        int a = 5;
        int b = 5;

        int c = a + b - 2;
        d = c * 2 + 3;
    }

    return d * 4;
}

int req2() {
    int d;
    if (0) {
        int a = 5;
        int b = 5;

        int c = a + b - 2;
        d = c * 2 + 3;
    }

    if (1) {
        d = 1;
        d = 2;
        d = 3;
        d = 4;
        d = 5;
    }

    return d * 4;
}