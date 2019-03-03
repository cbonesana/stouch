#ifndef BUTTONSYSTEM_H
#define BUTTONSYSTEM_H

#include <QCoreApplication>

#include "../Log/LogModule.h"
#include "SignalsModule.h"

#include <cstdio>
#include <ctime>

#define D_BUTTONS       0   // button pressed
#define D_REGIST_CLIENT 1   // register a new client
#define D_REMOVE_CLIENT 2   // remove a client
#define D_TOGGLE_BUTTON 3   // toggle button acquisition
#define D_ACKNOWLEDGE   4   // acknowledge
#define D_ERROR         -1  // error
#define D_STOP_CLIENT   5   // stop client signal

using namespace std;

extern SignalsModule *signalsModule;

void handler(int signal, siginfo_t *info, void *p);

void initSignals(QCoreApplication *a, pid_t parentpid, LogModule *logModule);

#endif // BUTTONSYSTEM_H
