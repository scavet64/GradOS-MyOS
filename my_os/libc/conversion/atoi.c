int atoi(char *str) 
{ 
    int result = 0; // Initialize result 
    int i;

    for (i = 0; str[i] != '\0'; ++i) {
        result = result*10 + str[i] - '0'; 
    }
   
    // return result. 
    return result; 
} 