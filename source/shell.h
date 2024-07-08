#include <limits.h> // For PATH_MAX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_LINE 1024
#define MAX_ARGS 64
#define BIN_PATH "./bin/"

extern char **environ;
char **env;
const char *builtin_commands[] = {
    "cd",      // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",    //  List all builtin commands in the shell
    "exit",    // Exits the shell
    "usage",   // Provides a brief usage guide for the shell and its built-in command
    "env",     // Lists all the environment variables currently set in the shell
    "setenv",  // Sets or modifies an environment variable for this shell session
    "unsetenv" // Removes an environment variable from the shell
};
/*
Handler of each shell builtin function
*/
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_usage(char **args);
int list_env(char **args);
int set_env_var(char **args);
int unset_env_var(char **args);

/*** This is array of functions, with argument char ***/
int (*builtin_command_func[])(char **) = {
    &shell_cd,     // builtin_command_func[0]: cd
    &shell_help,   // builtin_command_func[1]: help
    &shell_exit,   // builtin_command_func[2]: exit
    &shell_usage,  // builtin_command_func[3]: usage
    &list_env,     // builtin_command_func[4]: env
    &set_env_var,  // builtin_command_func[5]: setenv
    &unset_env_var // builtin_command_func[6]: unsetenv
};
// Helper function to figure out how many builtin commands are supported by the shell
int num_builtin_functions()
{
    return sizeof(builtin_commands) / sizeof(char *);
};



/////////////////////////////////////////////////////////////////////////////////

int shell_help(char **args)
{
    printf("The following commands are implemented within the shell:\n");
    for (int i = 0; i < num_builtin_functions(); i++)
    {
        printf("  %s\n", builtin_commands[i]);
    }
    return 0;
}
int shell_exit(char **args)
{
    exit(0);
    return 0;
}
int shell_usage(char **args)
{
    if (args[1] == NULL)
    {
        printf("Command not given. Type usage <command>.\n");
    }
    else if (strcmp(args[1], "cd") == 0)
    {
        printf("Type: cd directory_name to change the current working directory of the shell\n");
    }
    else if (strcmp(args[1], "help") == 0)
    {
        printf("Type: help for supported commands\n");
    }
    else if (strcmp(args[1], "exit") == 0)
    {
        printf("Type: exit to terminate the shell gracefully\n");
    }
    else if (strcmp(args[1], "usage") == 0)
    {
        printf("Type: usage cd/help/exit\n");
    }
    else if (strcmp(args[1], "env") == 0)
    {
        printf("Type: env to list all registered env variables\n");
    }
    else if (strcmp(args[1], "setenv") == 0)
    {
        printf("Type: setenv ENV=VALUE to set a new env variable\n");
    }
    else if (strcmp(args[1], "unsetenv") == 0)
    {
        printf("Type: unsetenv ENV to remove this env from the list of env variables\n");
    }
    else
    {
        printf("The command you gave: %s, is not part of the CSEShell's builtin command\n", args[1]);
    }

    return 0;
}

int shell_cd(char **args)
{
    char cwd[1024];
    // // strcat(cwd, "/");
    // strcat(cwd, args[1]);
    // printf("pathname: %s\n", cwd);
    if (args[1] != NULL)
    {
        if (chdir(args[1]) != 0)
        {
            printf("Directory not found.\n");
        }
        else
        {
            printf("current working directory: %s", getcwd(cwd, sizeof(cwd)));
        }
    }
    return 0;
}
int set_env_var(char **args)
{
    if (args[1] != NULL)
    {
        char *name = strtok(args[1], "=");
        char *alias = strtok(NULL, "=");
        setenv(name, alias, 1);
    }
    return 0;
}
int list_env(char **args)
{
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
    return 0;
}
int unset_env_var(char **args)
{
    unsetenv(args[1]);
    return 0;
}