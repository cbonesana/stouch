#include <cstdio>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <netdb.h>

#include <wiringPi.h>

using namespace std;

#define DAEMON_NAME     "stouchd"
#define RUNNING_DIR     "/opt/stouch/"
#define LOCK_FILE       "daemon/stouchd.lock"
#define LOG_FILE        "daemon/stouchd.log"
#define CLIENT_FILE     "daemon/client.pid"
#define MIN_WAIT_TIME   3

#define CHECK_TIME_ON   60
#define CHECK_TIME_OFF  10

/* Daemon commands */
#define D_BUTTONS       0   // button pressed
#define D_REGIST_CLIENT 1   // register a new client
#define D_OFFLINE_MODE  2   // remove a client
#define D_ONLINE_MODE   3   // toggle button acquisition
#define D_ACKNOWLEDGE   4   // acknowledge
#define D_STOP_CLIENT   5   // stop client signal
#define D_ERROR         -1  // error

#define S_POWERUP       "mpg123 /opt/stouch/sound/powerup.mp3"
#define S_POWERDOWN     "mpg123 /opt/stouch/sound/powerdown.mp3"
#define S_ERROR         "mpg123 /opt/stouch/sound/error.mp3"
#define S_OK            "mpg123 /opt/stouch/sound/send.mp3"
#define S_V_ONLINE      "mpg123 /opt/stouch/sound/voice_online.mp3"
#define S_V_OFFLINE     "mpg123 /opt/stouch/sound/voice_err_connection.mp3"

#define APPLICATION     "/opt/stouch/stouchapp > /dev/null"
#define ARGS            "-p"

#define SERVER_NAME     "www.tv-surf.com"
#define SERVER_PORT     "62001"


/*  code defensively, code defensively, code defensively! */

int lfp;    // lock file pointer
int cfp;    // client file poiner
int cmode;  // client mode
pid_t pid = 0, sid = 0, cpid = 0;

int checkTime = CHECK_TIME_ON;

time_t lastUpdate;

/* Daemon send signal function */

int sendSignal(int msg, int id){
    union sigval value;
    value.sival_int = msg;
    syslog(LOG_NOTICE, "Sent signal to process %d", id);

    if (kill(id,0) == 0){
        return sigqueue(id, SIGUSR1, value);
    } else {
        syslog(LOG_NOTICE, "Pid %d doesn't exist.", id);
        return -1;
    }
}

/* Daemon process function */
int process() {

//    syslog(LOG_NOTICE, "Check connection sequence initiated.");
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    // check if connections exist
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP stream socket

    if ((status = getaddrinfo(SERVER_NAME, SERVER_PORT, &hints, &servinfo)) != 0){
//        syslog(LOG_NOTICE, "getaddrinfo: test connection successfull.");
//    } else {
        syslog(LOG_NOTICE, "ERROR! getaddrinfo: test connection failed, %s (%d)", gai_strerror(status), status);
    }

    // check if program is running
//    if (sendSignal(D_ACKNOWLEDGE, cpid) == 0){
//        syslog(LOG_NOTICE, "Ping sent to stouchapp %d.", cpid);
//    } else {
//        syslog(LOG_NOTICE, "Unable to contact stouchapp %d. Try application restart.", cpid);
//        cpid = 0;
//    }

    // manage program
    if (cpid == 0 && status == 0){
        // launch program
        cpid      = fork();
        cmode     = D_ONLINE_MODE;
        checkTime = CHECK_TIME_ON;

        system(S_V_ONLINE);

        if (cpid == 0){
            // application children
            pid_t ppid = getppid();
            syslog(LOG_NOTICE, "Executing client process from daemon with pid = %d.", ppid);

            char buffer[255];
            sprintf(buffer, "%s %s %d &", APPLICATION, ARGS, ppid);
            system(buffer);
            exit(EXIT_SUCCESS);
        } else {
            // parent children
            syslog(LOG_NOTICE, "Client launched with pid = %d.", cpid);
        }

    } else if (status != 0 && cpid != 0) {
        // stop program

        if (cmode != D_OFFLINE_MODE){
            system(S_V_OFFLINE);
        }

        cmode     = D_OFFLINE_MODE;
        checkTime = CHECK_TIME_OFF;

        if (sendSignal(D_OFFLINE_MODE, cpid) == 0) {
            syslog(LOG_NOTICE, "Offline mode signal correctly sent to %d", cpid);
        } else {
            syslog(LOG_NOTICE, "Error sending offline mode signal to %d", cpid);
        }

    } else if (cpid != 0 && status == 0 && cmode == D_OFFLINE_MODE){

        if (cmode != D_ONLINE_MODE){
            system(S_V_ONLINE);
        }

        cmode     = D_ONLINE_MODE;
        checkTime = CHECK_TIME_ON;

        if (sendSignal(D_ONLINE_MODE, cpid) == 0) {
            syslog(LOG_NOTICE, "Online mode signal correctly sent to %d", cpid);
        } else {
            syslog(LOG_NOTICE, "Error sending online mode signal to %d", cpid);
        }

    }
    return 0;
}

/* signal handler functions */

/*!
 * \fn void SIGTERM_handler(int signal)
 * Handle the SIGTERM \a signal and clos safetly the daemon.
 *
 * \param sig signal recieved
 */
void SIGTERM_handler(int signal){
    if (signal == SIGTERM){
        /* finalize the server */
        syslog(LOG_NOTICE, "Daemon is shutting down.");
        system(S_POWERDOWN);

        if (cpid != 0){
            syslog(LOG_NOTICE, "Sent signal to process %d", cpid);
            if (sendSignal(D_STOP_CLIENT, cpid) == 0) {
                syslog(LOG_NOTICE, "Stop signal correctly sent to %d", cpid);
            } else {
                syslog(LOG_NOTICE, "Error sending stop signal to %d. Send TERM signal", cpid);
                char str[255];
                sprintf(str, "kill %d", cpid);
                syslog(LOG_NOTICE, "Kill main application with pid %d", cpid);
                system(str);
            }
        }
        if (lockf(lfp,F_ULOCK,0) < 0){
            syslog(LOG_NOTICE, "ERROR! Can't remove lock to %s%s", RUNNING_DIR, LOCK_FILE);
            closelog();
            exit(EXIT_FAILURE);
        }
        closelog();     // close log
        close(lfp);     // close lock file pointer
        exit(EXIT_SUCCESS);
    }
}

/*!
 * \fn void SIGUSR1_handler(int signal, siginfo_t *info, void *p)
 * Handle the SIGUSR1 \a signal and execute commands.
 *
 * \param signal signal received
 * \param info pointer struct
 * \param p not used
 */
void SIGUSR1_handler(int signal, siginfo_t *info, void *p){
    syslog(LOG_NOTICE, "Signal %d from pid %d ", signal, info->si_pid);
    if (signal == SIGUSR1){
        sigval_t value = info->si_value;
        switch(value.sival_int){
        case D_REGIST_CLIENT:
            cpid = info->si_pid;
            syslog(LOG_NOTICE, "New client with pid %d", info->si_pid);
            break;
        case D_ACKNOWLEDGE:
            syslog(LOG_NOTICE, "Client with pid %d acknowlodge", info->si_pid);
            break;
        case D_ERROR:
            syslog(LOG_NOTICE, "Client with pid %d raise a connection error", info->si_pid);
            cmode     = D_OFFLINE_MODE;
            checkTime = CHECK_TIME_OFF;

            if (sendSignal(D_OFFLINE_MODE, cpid) == 0) {
                syslog(LOG_NOTICE, "Offline mode signal correctly sent to %d", cpid);
            } else {
                syslog(LOG_NOTICE, "Error sending offline mode signal to %d", cpid);
            }
            break;
        default:
            break;
        }
    }
}

/* WirinPi buttons handler */
/*!
 * \fn interruptHandler
 * Handle the buttons event throught the GPIO pins. Each valid input have a 2 seconds
 * time where no other signal would be generated.
 */
void interruptHandler()
{
    syslog(LOG_NOTICE, "Button pression detected. Sending signals.");
    time_t currentTime = time(NULL);

    if (currentTime > lastUpdate + MIN_WAIT_TIME) {
        lastUpdate = currentTime;

        if (cpid != 0){
            if (sendSignal(D_BUTTONS,cpid) == 0 ){
                syslog(LOG_NOTICE, "Sent button pressed signal to process %d", cpid);
            } else {
                syslog(LOG_NOTICE, "Error sending button pressed signal to process %d", cpid);
            }
        }
    }
}

/*!
 * \brief main
 * To launch the daemon simply execute the program.
 * To terminate, send a SIGTERM signal to the daemon.
 * The process id is store in the lock file /opt/stouch/daemon/stouchd.lock
 *
 * \param argc not used
 * \param argv not used
 * \return 0 with success, otherwise -1
 */
int main(int argc, char *argv[])
{
    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_NOTICE, "Stouch daemon online");

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        syslog(LOG_NOTICE, "ERROR! Fork failed");
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
    /* Syslog used */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        syslog(LOG_NOTICE, "ERROR! SID failed");
        exit(EXIT_FAILURE);
    }
    syslog(LOG_NOTICE, "SID set to %d", sid);

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization */
    lastUpdate = time(NULL);

    /* Change the current working directory */
    if ((chdir(RUNNING_DIR)) < 0) {
        syslog(LOG_NOTICE, "ERROR! Directory %s doesn't exist", RUNNING_DIR);
        exit(EXIT_FAILURE);
    }
    syslog(LOG_NOTICE, "Running dir changed to %s", RUNNING_DIR);

    /* Try open the lock file */
    lfp = open(LOCK_FILE, O_RDWR | O_CREAT, 0644);
    if (lfp < 0){
        syslog(LOG_NOTICE, "ERROR! Can't open file %s%s", RUNNING_DIR, LOCK_FILE);
        exit(EXIT_FAILURE);
    }
    if (lockf(lfp,F_TLOCK,0) < 0) {
        syslog(LOG_NOTICE, "ERROR! Lock already used. A demon is working");
        exit(EXIT_FAILURE);
    }
    syslog(LOG_NOTICE, "Lock file %s%s open", RUNNING_DIR, LOCK_FILE);

    /* record pid to lockfile */
    char str[255];
    sprintf(str,"%d\n",getpid());
    write(lfp,str,strlen(str));

    /* create clientfile */
    cfp = open(CLIENT_FILE, O_RDWR | O_CREAT, 0666);
    if (cfp < 0){
        syslog(LOG_NOTICE, "ERROR! Can't open file %s%s", RUNNING_DIR, CLIENT_FILE);
        exit(EXIT_FAILURE);
    }
    sprintf(str,"%d\n",0);
    write(cfp,str,strlen(str));
    close(cfp);

    /* Catching signals */
    signal(SIGCHLD, SIG_IGN);           /* child terminate signal */
    signal(SIGTERM, SIGTERM_handler);   /* software termination signal from kill */

    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = SIGUSR1_handler;

    if (sigaction(SIGUSR1, &act, NULL)){                /* handler for client */
        syslog(LOG_NOTICE, "ERROR! Failed to set sigaction");
        exit(EXIT_FAILURE);
    }

    /* Wiring Pi setup */
    if(wiringPiSetup() < 0){
        syslog(LOG_NOTICE, "ERROR! Unable to setup wiringPi");
        exit(EXIT_FAILURE);
    }
    pinMode(0,INPUT);   // buttons color wire
    pinMode(1,OUTPUT);  // buttons black wire

    digitalWrite(1,HIGH);

//    pinMode(2,INPUT);   // shutdown pin

    /* ISR */
//    if (wiringPiISR(0, INT_EDGE_RISING, interruptHandler) < 0) {
//        syslog(LOG_NOTICE, "ERROR! Unable to setup ISR");
//        exit(EXIT_FAILURE);
//    }

    syslog(LOG_NOTICE, "Start main loop");
    system(S_POWERUP);

    /* The Big Loop */
    while (1) {
        process();
        sleep(CHECK_TIME_ON);
    }

    closelog();

    exit(EXIT_SUCCESS);
}
