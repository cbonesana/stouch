#include "SoundModule.h"

const QString SoundModule::playPowerUp          = "powerup";
const QString SoundModule::playPowerDown        = "powerdown";
const QString SoundModule::playButtonEvent      = "stouch";
const QString SoundModule::playError            = "error";
const QString SoundModule::playConnectionError  = "connectionerror";
const QString SoundModule::playSend             = "send";
const QString SoundModule::playTimer            = "timer";
const QString SoundModule::playTimerClick       = "timerclick";
const QString SoundModule::playWait             = "wait";
const QString SoundModule::playVoiceActive      = "voice_active";
const QString SoundModule::playVoiceCountdown   = "voice_countdown";
const QString SoundModule::playVoiceErrBracc    = "voice_err_bracc";
const QString SoundModule::playVoiceErrConn     = "voice_err_connection";
const QString SoundModule::playVoiceOffline     = "voice_offline";
const QString SoundModule::playVoiceOnline      = "voice_online";
const QString SoundModule::playVoicePrepare     = "voice_prepare";
const QString SoundModule::playCreativo         = "creative";


SoundModule::SoundModule(QString config, QString soundConfig, QObject *parent) : Module(config, "Sound Module", parent)
{
    soundDir = fileParser->parseFor("sound_dir", SOUND_DIR);

    soundFileParser = new FileParser(soundConfig);
}

SoundModule::~SoundModule()
{
    delete(soundFileParser);
}

void SoundModule::play(QString sound, int r)
{
    QString command = "mpg123 " % generatePath(sound) % " 2> /dev/null";
    qDebug() << command;
    for(int i=0; i<r; i++){
        system(qPrintable(command));
    }

    emit log(componentName, "Play sound " % generatePath(sound) % " " % QString::number(r) % " time(s).", "INFO ");
}

void SoundModule::play(QString sound1, QString sound2, QString sound3)
{
    QString command = "mpg123 " % generatePath(sound1) % " " % generatePath(sound2) % " " % generatePath(sound3) % " 2> /dev/null";

    system(qPrintable(command));

    emit log(componentName, "Play three sounds: " % generatePath(sound1) % ", " % generatePath(sound2) % " and " % generatePath(sound3) % ".", "INFO ");
}

QString SoundModule::generatePath(QString name)
{
    QString path = soundFileParser->parseFor(name);
    if (path == ""){
        path = soundDir % name % ".mp3";
    }
    return path;
}
