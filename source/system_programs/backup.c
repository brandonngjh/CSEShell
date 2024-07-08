#include "system_program.h"
#include <sys/stat.h>  // For mkdir
#include <sys/types.h> // For mode_t

void get_current_datetime(char *buffer, size_t size)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y%m%d%H%M%S", tm_info);
}

int main()
{
    char *backup_dir = getenv("BACKUP_DIR");
    if (backup_dir == NULL)
    {
        fprintf(stderr, "Error: BACKUP_DIR environment variable is not set.\n");
        return EXIT_FAILURE;
    }

    // Ensure the archive directory exists
    struct stat st = {0};
    if (stat("./archive", &st) == -1)
    {
        if (mkdir("./archive", 0700) != 0)
        {
            perror("Error: Failed to create archive directory");
            return EXIT_FAILURE;
        }
    }

    char datetime[20];
    get_current_datetime(datetime, sizeof(datetime));

    char zip_filename[PATH_MAX];
    snprintf(zip_filename, sizeof(zip_filename), "backup-%s.zip", datetime);

    char archive_path[PATH_MAX];
    if (snprintf(archive_path, sizeof(archive_path), "./archive/%s", zip_filename) >= sizeof(archive_path))
    {
        fprintf(stderr, "Error: Archive path is too long.\n");
        return EXIT_FAILURE;
    }

    char command[PATH_MAX * 3];
    if (snprintf(command, sizeof(command), "zip -r %s %s", zip_filename, backup_dir) >= sizeof(command))
    {
        fprintf(stderr, "Error: Command is too long.\n");
        return EXIT_FAILURE;
    }

    printf("Creating backup of '%s' at '%s'\n", backup_dir, archive_path);
    if (system(command) != 0)
    {
        fprintf(stderr, "Error: Failed to create the zip file.\n");
        return EXIT_FAILURE;
    }

    if (snprintf(command, sizeof(command), "mv %s ./archive/", zip_filename) >= sizeof(command))
    {
        fprintf(stderr, "Error: Command is too long.\n");
        return EXIT_FAILURE;
    }

    if (system(command) != 0)
    {
        fprintf(stderr, "Error: Failed to move the zip file to the archive directory.\n");
        return EXIT_FAILURE;
    }

    printf("Backup created successfully.\n");
    return EXIT_SUCCESS;
}
