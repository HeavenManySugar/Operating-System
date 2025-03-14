#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    struct stat statbuf;
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }
    if (stat(argv[1], &statbuf) < 0) {
        perror("stat");
        return 1;
    }
    if (S_ISDIR(statbuf.st_mode)) {
        printf("Cannot copy a directory\n");
        return 1;
    }
    FILE *src = fopen(argv[1], "r");
    if (src == NULL) {
        perror("fopen");
        return 1;
    }
    FILE *dst = fopen(argv[2], "w");
    if (dst == NULL) {
        perror("fopen");
        fclose(src);
        return 1;
    }
    char buffer[4096];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, n, dst) != n) {
            perror("fwrite");
            fclose(src);
            fclose(dst);
            return 1;
        }
    }
    fclose(src);
    fclose(dst);
    return 0;
}