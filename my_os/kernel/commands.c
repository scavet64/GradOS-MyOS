#include "util.h"
#include "../libc/include/conversion.h"
#include "../drivers/screen.h"

/**
 * Print the result of the operation after a tab
 */
void printResult(int res) {
    print("\t");
    printIntLn(res);
}

/**
 * Adds the two params together.
 */
int add(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a + b;
    printResult(result);
}

/**
 * Subtracts the second param from the first param.
 */
int sub(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a - b;
    printResult(result);
}

/**
 * Multiplys the two params.
 */
int mult(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a * b;
    printResult(result);
}

/**
 * Divides the first param by the second param.
 * Uses interger division.
 */
int div(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    if (b == 0) {
        printLn("\tError: Cannot divide by zero");
    } else {
        int result = a / b;
        printResult(result);
    }
}