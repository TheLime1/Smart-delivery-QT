#include <QtNetwork>
#include "sms.h"




QString twilio_phone_number = "+18154011965";


sms::sms(){


}
void sms::sendSMS(const QString &to, const QString &message) {
    QNetworkAccessManager *manager = new QNetworkAccessManager();


    QUrl url("https://api.twilio.com/2010-04-01/Accounts/ACd6a636fad3cc49b59bcbbc225d9713a3/Messages.json");
    QNetworkRequest request(url);


    QString credentials = "ACd6a636fad3cc49b59bcbbc225d9713a3:e5c2a38e91b5defa9cf89ba1e2cd4b26";
    QByteArray base64Credentials = credentials.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + base64Credentials);




    QByteArray postData;
    postData.append("From="+twilio_phone_number);
    postData.append("&To=" + to);
    postData.append("&Body=" + message);




    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");


    // Make the request
    QNetworkReply *reply = manager->post(request, postData);




    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "SMS sent successfully";
        } else {
            qDebug() << "Failed to send SMS:" << reply->errorString();
        }




        reply->deleteLater();
        manager->deleteLater();
    });
}
