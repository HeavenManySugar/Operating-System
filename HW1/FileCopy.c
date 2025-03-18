// 2.24

#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }
    struct stat statbuf;
    if (stat(argv[1], &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        printf("Cannot copy a directory\n");
        return 1;
    }
    FILE *fp1;
    fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
    FILE *fp2;
    fp2 = fopen(argv[2], "w");
    if (fp2 == NULL) {
        printf("Cannot open file %s\n", argv[2]);
        return 1;
    }
    ssize_t c;
    while ((c = fgetc(fp1)) != EOF) {
        fputc(c, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}