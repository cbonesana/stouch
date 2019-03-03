#ifndef SOUNDMODULE_H
#define SOUNDMODULE_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringBuilder>

#include <cstdlib>
#include <unistd.h>

#include "../FileParser.h"
#include "../Module.h"
#include "../config.h"

using namespace std;

class SoundModule : public Module
{
    Q_OBJECT

public:
    explicit SoundModule(QString config, QString soundConfig, QObject *parent = 0);
    ~SoundModule();

    static const QString playPowerUp;
    static const QString playPowerDown;
    static const QString playButtonEvent;
    static const QString playConnectionError;
    static const QString playError;
    static const QString playSend;
    static const QString playTimer;
    static const QString playTimerClick;
    static const QString playWait;
    static const QString playVoiceActive;
    static const QString playVoiceCountdown;
    static const QString playVoiceErrBracc;
    static const QString playVoiceErrConn;
    static const QString playVoiceOffline;
    static const QString playVoiceOnline;
    static const QString playVoicePrepare;
    static const QString playCreativo;

signals:
    void log(QString sender, QString message, QString priority);
    void finished();

public slots:
    void play(QString sound, int r);
    void play(QString sound1, QString sound2, QString sound3);
    
private:
    QString soundDir;

    FileParser *soundFileParser;

    QString generatePath(QString name);
};

#endif // SOUNDMODULE_H
