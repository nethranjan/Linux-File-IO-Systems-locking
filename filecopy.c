#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int in_fd, out_fd;
    ssize_t nread, nwritten;

    // Open source file
    in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    // Open destination file
    out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd == -1) {
        perror("Error opening destination file");
        close(in_fd);
        exit(EXIT_FAILURE);
    }

    // Copy contents
    while ((nread = read(in_fd, buffer, BUFFER_SIZE)) > 0) {

        char *out_ptr = buffer;
        ssize_t remaining = nread;

        while (remaining > 0) {
            nwritten = write(out_fd, out_ptr, remaining);

            if (nwritten <= 0) {
                perror("Error writing to destination file");
                close(in_fd);
                close(out_fd);
                exit(EXIT_FAILURE);
            }

            remaining -= nwritten;
            out_ptr += nwritten;
        }
    }

    if (nread < 0) {
        perror("Error reading source file");
    }

    close(in_fd);
    close(out_fd);

    return EXIT_SUCCESS;
}
