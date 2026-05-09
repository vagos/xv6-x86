#include "types.h"
#include "user.h"
#include "pstat.h"
#define N 5

int tickets[N] = {200, 100, 500, 50, 150};
int children[N];
struct pstat pstat;
int lottery;

int pindex(struct pstat *pstat, int pid) {
    for (int i = 0; i < NPROC; i++) {
        if (pstat->pid[i] == pid) {
            return i;
        }
    }
    return -1;
}

void fork_children() {
    for (int i = 0; i < N; i++) {
        int fpid = fork();

        if (fpid == 0) {
            settickets(tickets[i]);

            for (;;) {
            }

        } else if (fpid != -1) {
            children[i] = fpid;
        } else {
            printf(1, "\nFailed to fork children processes\n");
            for (int j = 0; j < i; j++) {
                kill(children[j]);
                wait();
            }
            exit();
        }
    }
    printf(1, "\nForked %d children ", N);
}

void kill_children() {
    for (int i = 0; i < N; i++) {
        kill(children[i]);
        wait();
    }
}

void print_info() {
    int index[N] = {-1};
    int ticks[N] = {-1};
    int tticks = 0;

    for (int i = 0; i < N; i++) {
        index[i] = pindex(&pstat, children[i]);

        if (index[i] == -1) {
            printf(1, "Failed to get process info\n");
            exit();
        }

        ticks[i] = pstat.ticks[index[i]];
        tticks += ticks[i];
    }

    printf(1, "(real %d)\n\n", tticks);

    for (int i = 0; i < N; i++) {
        int cpu1 = ticks[i] / (tticks / 100);
        int cpu2 = ticks[i] / (tticks / 1000) % 10;

        printf(1, "PID: %d\tTICKETS: %d\tTICKS: %d\tCPU: %d.%d%%\n",
               children[i], tickets[i], ticks[i], cpu1, cpu2);
    }
    printf(1, "\n");
}

void main(int argc, char *argv[]) {
    settickets(1000000);

    fork_children();

    printf(1, "to share ~3000 ticks ");

    sleep(3000);

    if (getpinfo(&pstat) == -1) {
        printf(1, "\nFailed to get pinfo\n");
        kill_children();
        exit();
    }

    kill_children();
    print_info();
    exit();
}
