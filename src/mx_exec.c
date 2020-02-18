#include "ush.h"

int mx_exec(t_process *process, char *filename, char **argv, char **env) {
    t_list **list = mx_get_list_procs();

    mx_disable_canon();
    process->status = posix_spawn(&process->pid, filename,
                         &process->actions, &process->attrs, argv, env);
    // waitpid(process->pid, &status, WUNTRACED);
    if (waitpid(process->pid, &process->status, WUNTRACED) != -1) {
        if (WIFSTOPPED(process->status)) {
            mx_push_back(list, process);
        }
    }
    mx_enable_canon();
    return process->status;
}

// static void test() { // create a new process;
//     mx_disable_canon();
//     extern char **environ;
//     pid_t pid;
//     sigset_t set;
//     char *argv[] = {"ls", "-R", "/", NULL};
//     int status;
//     sigemptyset(&set);
//     sigaddset(&set, SIGINT);
//     sigaddset(&set, SIGTSTP);
//     // sigfillset(&set);
//     posix_spawnattr_t attr;
//     posix_spawnattr_init(&attr);
//     posix_spawnattr_setsigdefault(&attr, &set);
//     posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGDEF | POSIX_SPAWN_SETSIGMASK);
//     status = posix_spawn(&pid, "/bin/ls", NULL, &attr, argv, environ);
//     if (status) {
//         printf("ERROR: posix_spawn: %s\n", strerror(status));
//         exit(EXIT_FAILURE);
//     }
//     printf("PID: %i\n", pid);
//     if (waitpid(pid, &status, WUNTRACED) != -1) {
//         printf("EXIT: %i\n", status);
//         if (WIFSTOPPED(status)) {
//             //if ctrl+z
//         }
//     }
//     else {
//         perror("ERROR: waitpid");
//     }
//     mx_enable_canon();
// }
