// Include the shell header file for necessary constants and function declarations
#include "shell.h"
#include "stdbool.h"
#include <sys/time.h>     // For struct timeval
#include <sys/resource.h> // For getrusage()

// Function to read a command from the user input
void read_command(char **cmd)
{
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE)
    {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL)
  {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++)
  {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time)
  {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }
  fflush(stdout); // Flush the output buffer
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s $$ ", cwd); // Print the shell prompt
}

void execute_command(char *cmd[])
{
  int status, child_exit_status;
  pid_t pid = fork();
  if (pid == -1)
  {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0)
  {
    // Child process
    execvp(cmd[0], cmd);
    // If execvp returns, an error has occurred
    printf("Command %s not found\n", cmd[0]);
    _exit(0);
  }
  else
  {
    // parent process need wait for child and retrieve exit status
    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status))
    {
      child_exit_status = WEXITSTATUS(status);
    }
  }
}

void load_shellrc(const char *filepath)
{
  // open file to read
  FILE *file = fopen(filepath, "r");
  if (file == NULL)
  {
    printf("No file detected");
    return;
  }
  char line[MAX_LINE];
  while (fgets(line, sizeof(line), file))
  {
    // Remove trailing newline character
    line[strcspn(line, "\n")] = '\0';

    // If the line starts with "PATH", set the PATH environment variable
    if (strncmp(line, "PATH=", 5) == 0)
    {
      char *path_value = line + 5;
      if (setenv("PATH", path_value, 1) == -1)
      {
        perror("Failed to set PATH");
      }
    }
    else if (line[0] != '\0')
    {
      // Treat the line as a command to be executed
      char *cmd[100];
      int i = 0;
      cmd[i] = strtok(line, " ");
      while (cmd[i] != NULL && i < 99)
      {
        i++;
        cmd[i] = strtok(NULL, " ");
      }
      cmd[i] = NULL; // Null-terminate the command array

      // Execute the command
      execute_command(cmd);

      //   for (int i = 0; cmd[i] != NULL; i++) {
      //     free(cmd[i]);
      // }
    }
  }
  fclose(file);
}

// Function to display resource usage
void display_resource_usage(struct rusage *usage)
{
  printf("Resource usage:\n");
  printf("User CPU time used: %ld.%06ld seconds\n", usage->ru_utime.tv_sec, usage->ru_utime.tv_usec);
  printf("System CPU time used: %ld.%06ld seconds\n", usage->ru_stime.tv_sec, usage->ru_stime.tv_usec);
  printf("Maximum resident set size: %ld KB\n", usage->ru_maxrss);
}

// The main function where the shell's execution begins
int main(void)
{
  type_prompt();     // Display the prompt
  load_shellrc(".cseshellrc");
  int ret;
  while (true)
  {
    type_prompt();
    ret = 1;
    env = environ;
    // Define an array to hold the command and its arguments
    char *cmd[MAX_ARGS];
    int child_status;
    pid_t pid;
    read_command(cmd); // Read a command from the user
    if (!(*cmd)) // if no command input then continue to next iteration to prompt again
      continue;
    // If the command is "exit", break out of the loop to terminate the shell

    for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
    {
      if (strcmp(cmd[0], builtin_commands[command_index]) == 0) // Assume args[0] contains the first word of the command
      {
        // We will create new process to run the function with the specific command except for builtin commands.
        // These have to be done by the shell process.
        ret = (*builtin_command_func[command_index])(cmd);
        break;
      }
    }
    if (ret == 0)
      continue;
    pid = fork(); // fork to child ps

    if (pid < 0)
    {
      printf("fork failed :(..");
    }
    else if (pid == 0)
    { // in child process
      // Formulate the full path of the command to be executed
      char full_path[PATH_MAX];
      char cwd[1024];
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
      }
      else
      {
        printf("Failed to get current working directory.");
        exit(1);
      }

      execv(full_path, cmd);
      // If execv returns, command execution has failed
      printf("Command %s not found\n", cmd[0]);
      exit(0);

      // Free the allocated memory for the command arguments before exiting
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
      }
      memset(cwd, '\0', sizeof(cwd)); // clear the cwd array
      return 0;
    }
    else
    {
      // Wait for the child process to finish
      waitpid(pid, &child_status, WUNTRACED);

      // Get resource usage of the child process
      struct rusage usage;
      getrusage(RUSAGE_CHILDREN, &usage);
      display_resource_usage(&usage);
    }
  }
  return 0;
}
