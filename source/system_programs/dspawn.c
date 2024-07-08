#include "system_program.h"

char output_file_path[PATH_MAX];

static int daemon_work()
{
    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1)
    {
        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(output_file_path, "a");
        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.\n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

static void create_daemon()
{
    pid_t pid;

    // First fork
    pid = fork();

    if (pid < 0) // Error
    {
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) // Parent
    {
        exit(EXIT_SUCCESS); // exit(1)
    }

    setsid(); // Make child session leader

    // Ignore signal sent from child to parent process
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Second fork
    pid = fork();

    if (pid < 0) // Error
    {
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) // Parent
    {
        exit(EXIT_SUCCESS);
    }

    umask(0); // Set file permission

    chdir("/");

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    // Attach file descriptors 0, 1, and 2 to /dev/null
    open("/dev/null", O_RDWR); // stdin
    dup(0);                    // stdout
    dup(0);                    // stderr

    daemon_work();
}

int main()
{
    // Setup path
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        return 1;
    }
    strcat(output_file_path, "/dspawn.log");

    create_daemon();
    return 0;
}