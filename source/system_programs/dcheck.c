#include "system_program.h"

int main()
{
    FILE *fp;
    char buffer[128];
    int daemon_count = 0;

    const char *command = "ps -efj | grep dspawn | grep -Ev 'tty|pts' | wc -l";

    fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Failed to run command");
        exit(EXIT_FAILURE);
    }

    // Read the output one line at a time
    if (fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
    {
        daemon_count = atoi(buffer);
    }

    pclose(fp);

    printf("Number of running dspawn daemon processes: %d\n", daemon_count);

    return 0;
}
