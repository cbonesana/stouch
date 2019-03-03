#include <iostream>
#include <fstream>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FILE "/opt/stouch/daemon/stouchd.lock"

using namespace std;

int main(int argc, char *argv[])
{
    string str;

    ifstream f;
    f.open(SERVER_FILE);
    f >> str;
    f.close();

    pid_t pid = atoi(str.c_str());

    cout << "Server pid: " << pid << endl;

    union sigval value;
    value.sival_int = 42;

    sigqueue(pid,SIGUSR1,value);

    exit(EXIT_SUCCESS);
}

