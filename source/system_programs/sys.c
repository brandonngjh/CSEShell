#include "system_program.h"

// Function to get the username of the currently logged-in user
char *get_username()
{
    struct passwd *pw = getpwuid(getuid());
    return pw ? pw->pw_name : "Unknown";
}

// Function to get the total memory size in MB
long get_total_memory()
{
    long total_memory = -1;
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if (meminfo != NULL)
    {
        char line[256];
        while (fgets(line, sizeof(line), meminfo))
        {
            if (sscanf(line, "MemTotal: %ld kB", &total_memory) == 1)
            {
                total_memory /= 1024; // Convert from KB to MB
                break;
            }
        }
        fclose(meminfo);
    }
    return total_memory;
}

// Function to get the number of CPUs
int get_cpu_count()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int main()
{
    struct utsname buffer;
    if (uname(&buffer) != 0)
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    printf(COLOR_CYAN "System Information:\n" COLOR_RESET);
    printf(COLOR_GREEN "Operating System: " COLOR_RESET "%s\n", buffer.sysname);
    printf(COLOR_GREEN "Node Name: " COLOR_RESET "%s\n", buffer.nodename);
    printf(COLOR_GREEN "Kernel Release: " COLOR_RESET "%s\n", buffer.release);
    printf(COLOR_GREEN "Kernel Version: " COLOR_RESET "%s\n", buffer.version);
    printf(COLOR_GREEN "Machine: " COLOR_RESET "%s\n", buffer.machine);
    printf(COLOR_GREEN "Total Memory: " COLOR_RESET "%ld MB\n", get_total_memory());
    printf(COLOR_GREEN "CPU Count: " COLOR_RESET "%d\n", get_cpu_count());
    printf(COLOR_GREEN "Current User: " COLOR_RESET "%s\n", get_username());

    return 0;
}
