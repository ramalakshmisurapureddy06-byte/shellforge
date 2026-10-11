#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/utsname.h>
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "jobs.h"
#include "builtin.h"
#include <signal.h>
#include <sys/wait.h>
#include <signal.h>
#include "job_control.h"

/* =========================================================
   BUILTIN: cd
   ========================================================= */

static int builtin_cd(command_t *cmd)
{
    const char *directory;

    /*
     * cd with no argument
     * changes to the user's HOME directory.
     */
    if (cmd->argc == 1)
    {
        directory = getenv("HOME");

        if (directory == NULL)
        {
            fprintf(stderr,
                    "cd: HOME not set\n");

            return -1;
        }
    }
    else if (cmd->argc == 2)
    {
        /*
         * cd has one directory argument.
         */
        directory = cmd->argv[1];
    }
    else
    {
        /*
         * Too many arguments.
         */
        fprintf(stderr,
                "cd: too many arguments\n");

        return -1;
    }

    /*
     * Change the current working directory.
     */
    if (chdir(directory) != 0)
    {
        perror("cd");

        return -1;
    }

    return 0;
}


/* =========================================================
   BUILTIN: pwd
   ========================================================= */

static int builtin_pwd(command_t *cmd)
{
    char current_directory[4096];

    /*
     * pwd does not need arguments.
     */
    if (cmd->argc > 1)
    {
        fprintf(stderr,
                "pwd: too many arguments\n");

        return -1;
    }

    /*
     * Get current working directory.
     */
    if (getcwd(current_directory,
               sizeof(current_directory)) == NULL)
    {
        perror("pwd");

        return -1;
    }

    /*
     * Display current directory.
     */
    printf("%s\n", current_directory);

    return 0;
}


/* =========================================================
   BUILTIN: echo
   ========================================================= */

static int builtin_echo(command_t *cmd)
{
    /*
     * Start from argv[1].
     *
     * argv[0] contains "echo".
     */
    for (int i = 1; i < cmd->argc; i++)
    {
        printf("%s", cmd->argv[i]);

        /*
         * Print a space between arguments.
         */
        if (i < cmd->argc - 1)
        {
            printf(" ");
        }
    }

    printf("\n");

    return 0;
}


/* =========================================================
   BUILTIN: exit
   ========================================================= */

static int builtin_exit(command_t *cmd)
{
    /*
     * Basic version:
     *
     * exit
     */
    if (cmd->argc > 1)
    {
        fprintf(stderr,
                "exit: too many arguments\n");

        return -1;
    }

    /*
     * Tell the main shell loop to terminate.
     */
    return 1;
}


/* =========================================================
   CHECK WHETHER COMMAND IS A BUILTIN
   ========================================================= */
    
   
int is_builtin(const command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "echo") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "jobs") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "fg") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "bg") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "mydate") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "myuser") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "mydir") == 0)
        return 1;
    if (strcmp(cmd->argv[0], "mysystem") == 0)
        return 1;

    return 0;
}



/* =========================================================
   EXECUTE BUILTIN
   ========================================================= */

int execute_builtin(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return -1;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return builtin_cd(cmd);
    if (strcmp(cmd->argv[0], "pwd") == 0)
        return builtin_pwd(cmd);
    if (strcmp(cmd->argv[0], "echo") == 0)
        return builtin_echo(cmd);
    if (strcmp(cmd->argv[0], "exit") == 0)
        return builtin_exit(cmd);
    if (strcmp(cmd->argv[0], "jobs") == 0)
        return builtin_jobs(cmd);
    if (strcmp(cmd->argv[0], "fg") == 0)
        return builtin_fg(cmd);
    if (strcmp(cmd->argv[0], "bg") == 0)
        return builtin_bg(cmd);
    if (strcmp(cmd->argv[0], "mydate") == 0)
        return builtin_mydate(cmd);
    if (strcmp(cmd->argv[0], "myuser") == 0)
        return builtin_myuser(cmd);
    if (strcmp(cmd->argv[0], "mydir") == 0)
        return builtin_mydir(cmd);
    if (strcmp(cmd->argv[0], "mysystem") == 0)
        return builtin_mysystem(cmd);

    return -1;
}


// built in jobs 

int builtin_jobs(command_t *cmd)
{
    (void)cmd;

    jobs_print();

    return 0;
}


// implement fg

int builtin_fg(command_t *cmd)
{
    int job_id;

    job_t *job;

    int status;


    if (cmd->argc < 2)
    {
        printf("fg: usage: fg <job number>\n");

        return 1;
    }


    job_id = atoi(cmd->argv[1]);


    job = job_find(job_id);


    if (job == NULL)
    {
        printf(
            "fg: no such job: %d\n",
            job_id
        );

        return 1;
    }


    /*
     * Give terminal to job.
     */

    give_terminal_to(job->pgid);


    /*
     * Continue stopped process group.
     */

    if (job->state == JOB_STOPPED)
    {
        kill(
            -job->pgid,
            SIGCONT
        );
    }


    job_continue(job->pgid);


    /*
     * Wait for the process group.
     */

    while (1)
    {
        pid_t result =
            waitpid(
                -job->pgid,
                &status,
                WUNTRACED
            );


        if (result < 0)
        {
            break;
        }


        if (WIFSTOPPED(status))
        {
            job_stop(job->pgid);

            break;
        }


        if (WIFEXITED(status) ||
            WIFSIGNALED(status))
        {
            job_done(job->pgid);

            break;
        }
    }


    /*
     * Give terminal back to Shellforge.
     */

    take_terminal_back();


    /*
     * Remove completed job.
     */

    if (job->state == JOB_DONE)
    {
        job_remove(job_id);
    }


    return 0;
}

// implement bg 

int builtin_bg(command_t *cmd)
{
    int job_id;
    job_t *job;

    if (cmd->argc < 2)
    {
        fprintf(stderr, "bg: usage: bg <job number>\n");
        return 1;
    }

    job_id = atoi(cmd->argv[1]);

    job = job_find(job_id);

    if (job == NULL)
    {
        fprintf(stderr, "bg: job not found: %d\n", job_id);
        return 1;
    }

    if (kill(-job->pgid, SIGCONT) == -1)
    {
        perror("bg: kill");
        return 1;
    }

    job_continue(job->pgid);

    printf("[%d] %s &\n", job->job_id, job->command);

    return 0;
}


/* Display current date and time */
int builtin_mydate(command_t *cmd)
{
    (void)cmd;

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char buffer[100];

    if (local == NULL)
    {
        perror("mydate");
        return 1;
    }

    if (strftime(buffer, sizeof(buffer),
                 "%a %b %d %H:%M:%S %Y", local) == 0)
    {
        fprintf(stderr, "mydate: formatting failed\n");
        return 1;
    }

    printf("%s\n", buffer);
    return 0;
}

/* Display current username */
int builtin_myuser(command_t *cmd)
{
    (void)cmd;

    const char *user = getenv("USER");

    if (user == NULL)
        user = getenv("LOGNAME");

    if (user == NULL)
    {
        fprintf(stderr, "myuser: username not found\n");
        return 1;
    }

    printf("Current user: %s\n", user);
    return 0;
}

/* Display current directory */
int builtin_mydir(command_t *cmd)
{
    (void)cmd;

    char directory[4096];

    if (getcwd(directory, sizeof(directory)) == NULL)
    {
        perror("mydir");
        return 1;
    }

    printf("Current directory: %s\n", directory);
    return 0;
}

/* Display operating system information */
int builtin_mysystem(command_t *cmd)
{
    (void)cmd;

    struct utsname info;

    if (uname(&info) == -1)
    {
        perror("mysystem");
        return 1;
    }

    printf("Operating System: %s\n", info.sysname);
    printf("Computer Name: %s\n", info.nodename);
    printf("Kernel Release: %s\n", info.release);
    printf("Machine Type: %s\n", info.machine);

    return 0;
}
