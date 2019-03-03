#include "SignalsHandlers.h"
#include <signal.h>
#include <unistd.h>

SignalsModule *signalsModule;

QCoreApplication *app;

void handler(int signal, siginfo_t *info, void *p)
{
    if (signal == SIGUSR1){
        int value = info->si_value.sival_int;
        switch(value){
        case D_BUTTONS:
            signalsModule->makeViewImage();
            break;
        case D_STOP_CLIENT:
            signalsModule->shutDownEverything();
            app->quit();
            break;
        case D_OFFLINE_MODE:
            signalsModule->sendOfflineMode();
            break;
        case D_ONLINE_MODE:
            signalsModule->sendOnlineMode();
            break;
        case D_ACKNOWLEDGE:
            signalsModule->sendAcknowledge();
            break;
        }
    }
}

void initSignals(QCoreApplication *a, pid_t parentpid, LogModule *logModule)
{
    app = a;
    if (signalsModule == NULL) {
        signalsModule = new SignalsModule(CONFIG_FILE, logModule, parentpid, &handler);
    }
}
