#include "HTTPResponse.h"

/*!
 * \fn HTTPResponse::HTTPResponse
 *
 * Constructs an HTTPResponse object from a JSON string.
 *
 * \param response a valid JSON string in a QByteArray with UTF8 encoding
 * \param parent
 */
HTTPResponse::HTTPResponse(QByteArray response, QObject *parent) : QObject(parent)
{
    doc = QJsonDocument::fromJson(response);
    jsonObject = doc.object();
}

/*!
 * \fn HTTPResponse::HTTPResponse
 *
 * Constructs an HTTPResponse object from a JSON string.
 *
 * \param response a valid JSON string.
 * \param parent
 */
HTTPResponse::HTTPResponse(QString response, QObject *parent) : QObject(parent)
{
    doc = QJsonDocument::fromJson(response.toUtf8());
    jsonObject = doc.object();
}

/*!
 * \fn HTTPResponse::get
 * \param key a key in the JSON object.
 * \return the value associated to the key in the JSON object, otherwise a void QString.
 */
QString HTTPResponse::get(QString key)
{
    if (jsonObject.contains(key)) {
        return jsonObject.value(key).toString();
    }
    return "";
}


