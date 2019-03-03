#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QObject>
#include <QString>
#include <QByteArray>

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include <cstdio>

/*!
 * \class HTTPResponse
 * \brief The HTTPResponse class is used to store a JSON object.
 *
 * The HTTPResponse class is a build around a JSON object and it provides an easy way to access the information in the JSON object.
 */
class HTTPResponse : public QObject
{
    Q_OBJECT

public:
    HTTPResponse(QByteArray response, QObject *parent=0);
    HTTPResponse(QString response, QObject *parent=0);

    QString get(QString key);

private:
    QJsonDocument doc;
    QJsonObject jsonObject;
};

#endif // HTTPRESPONSE_H
