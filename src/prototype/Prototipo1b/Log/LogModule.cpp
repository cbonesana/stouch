#include "LogModule.h"

LogModule::LogModule(QString config, QObject *parent) : Module(config, "LogModule", parent)
{
    QDate currentDate = QDate::currentDate();

    QString filename = fileParser->parseFor("log_name", LOG_NAME);

    logFileName = filename % "_" % currentDate.toString("yyyy-MM-dd") % ".log";
    logFile = new QFile(logFileName);

}

LogModule::~LogModule()
{
    if (logFile->isOpen()){
        logFile->close();
    }
    delete(logFile);
}

void LogModule::logging(QString sender, QString message, QString priority)
{
    bool status = logFile->open(QFile::Append | QFile::Text);
    if (!status) {
        QString err = "Error opening Log file " % logFileName % "\n";
        emit error(err);
    }

    QTime currentTime = QTime::currentTime();
    QDate currentDate = QDate::currentDate();

    QString logEntry = "[" % priority % "]\t"
            % currentDate.toString("yyyy-MM-dd") % " "
            % currentTime.toString("hh:mm:ss") % "\t"
            % sender % ": "
            % message %"\n";

    if (priority != "INFO "){
        emit logMessage(logEntry);      // this signal is used to log on server, via raspberry
    }

    QTextStream out(logFile);

    out << logEntry;

    logFile->close();
}
