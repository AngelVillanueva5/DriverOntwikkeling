#include <stdio.h>


int main() {
FILE *fptr;
fptr = fopen("/dev/hello_driver", "w");
fprintf(fptr, "opgave 3 text");
fclose(fptr);

return 0;
}
