#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

long total_bytes = 0;

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char *full_path;
        asprintf(&full_path, "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full_path, &st) == -1) {
            perror(full_path);
            free(full_path);
            continue;
        }

        if (S_ISREG(st.st_mode)) {
            printf("%ld:%s\n", st.st_size, full_path);
            total_bytes += st.st_size;
        } else if (S_ISDIR(st.st_mode)) {
            printf("dir %s\n", full_path);
            list_directory(full_path);
        }

        free(full_path);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *start_path = ".";
    if (argc > 1) {
        start_path = argv[1];
    }

    printf("Assignment 7 - Thee file listing program by Jakob Hodgson\n");
    printf("dir %s\n", start_path);
    list_directory(start_path);
    printf("Total file spaced used: %ld\n", total_bytes);
    return 0;
}
