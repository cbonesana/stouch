#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>


using namespace std;

#define DAEMON_NAME "mydaemon"
#define RUNNING_DIR "/opt/stouch/daemon"
#define LOCK_FILE "daemon.lock"
#define LOG_FILE "daemon.log"

/*  code defensively, code defensively, code defensively! */

int lfp;
pid_t pid, sid;

vector<int> clients;

/* signal handler functions */

/*!
 * \brief SIGTERM_handler
 * \param sig
 */
void SIGTERM_handler(int signal){
    if (signal == SIGTERM){
        /* finalize the server */
        syslog(LOG_NOTICE, "Daemon is shutting down.");
        if (lockf(lfp,F_ULOCK,0) < 0){
            syslog(LOG_ERR, "Can not remove lock to %s%s", RUNNING_DIR, LOCK_FILE);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}

/*!
 * \brief register_handler
 */
void SIGUSR1_handler(int signal, siginfo_t *info, void *p){
    if (signal == SIGUSR1){
        sigval_t value = info->si_value;
        if (value.sival_int == 42){
            syslog(LOG_NOTICE, "YUPPI!!!");
        }
        syslog(LOG_NOTICE, "Client request with signal %d from pid %d (%d)", signal, info->si_pid, value.sival_int);
        clients.push_back(info->si_pid);
    }
}

int main(int argc, char *argv[])
{
    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Entering Daemon");

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        syslog(LOG_ERR, "Fork failed");
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        syslog(LOG_NOTICE, "Fork successfull");
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);

    /* Open any logs here */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_ERR, "SID failed");
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization */

    /* Change the current working directory */
    if ((chdir(RUNNING_DIR)) < 0) {
        syslog(LOG_ERR, "Directory %s doesn't exist", RUNNING_DIR);
        exit(EXIT_FAILURE);
    }

    lfp = open(LOCK_FILE, O_RDWR|O_CREAT, 0644);
    if (lfp < 0){
        syslog(LOG_ERR, "Can not open file daemon.lock");
        exit(EXIT_FAILURE);
    }
    if (lockf(lfp,F_TLOCK,0) < 0) {
        syslog(LOG_INFO, "Lock already used. A demon is working");
        exit(EXIT_SUCCESS);
    }

    char str[255];
    sprintf(str,"%d\n",getpid());

    /* record pid to lockfile */
    write(lfp,str,strlen(str));

    /* Catching signals */
    signal(SIGCHLD, SIG_IGN);           /* child terminate signal */
    signal(SIGTERM, SIGTERM_handler);   /* software termination signal from kill */

    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = SIGUSR1_handler;

    sigaction(SIGUSR1, &act, NULL);     /* handler for client registration */

    /* The Big Loop */
    while (1) {
        sleep(1);
    }

    closelog();

    exit(EXIT_SUCCESS);
}
