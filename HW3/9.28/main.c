#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <virtual_address>\n", argv[0]);
        return 1;
    }

    unsigned int virtual_address = (unsigned int)strtoul(argv[1], NULL, 10);

    unsigned int page_size = 4096;
    unsigned int offset = virtual_address % page_size;
    unsigned int page_number = virtual_address / page_size;

    printf("The virtual address %u contains:\n", virtual_address);
    printf("Page number = %u\n", page_number);
    printf("Offset = %u\n", offset);

    return 0;
}