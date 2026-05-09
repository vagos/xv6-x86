#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "pstat.h"

#define TIMESTEPS 100
#define LTICKS(x) (x * 1000000)

/* A function to spend some CPU cycles on */
void 
spin(int J)
{
    int i = 0;
    int j = 0;
    int k = 0;
    for (i = 0; i < 50; i++) 
    {
        for (j = 0; j < J; j++) 
        {
            k = j % 10;
        }
    }

    (void)k; /* unused variable is unused :) */
}

#define N_C_PROCS 3

/* Print information about each of the running processes */

void
print_info(struct pstat *pstat, int j)
{
    printf(1, "%d\t%d\t", pstat->pid[j], pstat->ticks[j]);

    if (pstat->inuse[j] == 1) 
    {
        printf(1, "YES");
    }
    else 
    {
        printf(1, "NO");
    }

    printf(1, "\n");
}

/* Return the index of a process inside the pstat array */
int
find_pid(struct pstat *pstat, int pid)
{
    int i;
    for (i = 0; i < NPROC; i++)
    {
        if (pstat->pid[i] == pid) 
        {
            return i;
        }
    }
    return -1;
}

// Uncomment the below line after you've implemented the "settickets" system call
// Don't forget to do the same in lotteryschedtest.c
// #define TICKETS 

int
main(int argc, char *argv[])
{

    int pid_chds[N_C_PROCS];

    int n_tickets[N_C_PROCS]={2,1,300};
    pid_chds[0] = getpid();

#ifdef TICKETS
    settickets(n_tickets[0]);
#endif

    int i; 
    for (i = 1; i < N_C_PROCS; i++) 
    {
        if ((pid_chds[i] = fork()) == 0) 
        {
#ifdef TICKETS
            settickets(n_tickets[i]);
#endif
            int n_spin = LTICKS(5);
            spin(n_spin);
            exit();
        }
    }

    struct pstat pstat;
    int t = 0;

    printf(1, "PIDs of child processes:\n");
    for (i = 0; i < N_C_PROCS; i++) 
    {
        printf(1, "- pid %d\n", pid_chds[i]);
    }
    printf(1, "\n");

    printf(1, "PID\tTICKS\tIN USE\n");
    
    // int n_time = atoi(argv[1]); /* You can pass the number of time-steps as a command line argument if you uncomment this. Hard-coded for now. */
    int n_time = TIMESTEPS;

    spin(LTICKS(1));

    /* Every few seconds, print the process information. */
    while (t < n_time) 
    {
        
        if (getpinfo(&pstat) != 0) 
        {
            printf(1, "getpinfo failed\n");
            goto exit;
        }

        int j; int pid;
        for (i = 0; i < N_C_PROCS; i++)
        {
            pid = pid_chds[i];
            j = find_pid(&pstat, pid);
            print_info(&pstat, j);
        }

        int all_done = 1;
        for (i = 1; i < N_C_PROCS; i++)
        {
            j = find_pid(&pstat, pid_chds[i]);
            all_done &= !pstat.inuse[j];
        }
        if (all_done) break;

        spin(LTICKS(1));
        t++;
        printf(1, "\n");
    }

    /* Finally, kill all child processes. */
exit:
    for (i = 1; pid_chds[i] > 0 && i < N_C_PROCS; i++) 
    {
        kill(pid_chds[i]);
    }
    while(wait() != -1);

    exit();
}
