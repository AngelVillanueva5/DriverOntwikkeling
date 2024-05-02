#include <stdio.h>


int main() {
FILE *fptr;
fptr = fopen("/home/angel/DriverOntwikkeling/test.txt", "r");

char myString[10];

fgets(myString, 10, fptr);
fclose(fptr);
return 0;
}

