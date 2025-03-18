// 3.27

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

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

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if (pid == 0) { // Child process
        close(pipefd[1]);
        FILE *fp2;
        fp2 = fopen(argv[2], "w");
        if (fp2 == NULL) {
            printf("Cannot open file %s\n", argv[2]);
            return 1;
        }
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, bytesRead, fp2);
        }
        fclose(fp2);
        close(pipefd[0]);

    } else { // Parent process
        close(pipefd[0]);
        FILE *fp1;
        fp1 = fopen(argv[1], "r");
        if (fp1 == NULL) {
            printf("Cannot open file %s\n", argv[1]);
            return 1;
        }
        ssize_t c;
        while ((c = fgetc(fp1)) != EOF) {
            write(pipefd[1], &c, 1);
        }
        fclose(fp1);
        close(pipefd[1]);
    }
    return 0;
}