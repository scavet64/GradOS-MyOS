#include "util.h"
#include "../drivers/screen.h"

int add(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a + b;
    printIntLn(result);
}

int sub(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a - b;
    printIntLn(result);
}

int mult(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a * b;
    printIntLn(result);
}

int div(char *parm1, char *parm2){
    //Convert to ints
    int a = atoi(parm1);
    int b = atoi(parm2);

    int result = a / b;
    printIntLn(result);
}