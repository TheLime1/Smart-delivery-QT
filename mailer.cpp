#include "mailer.h"
#include"mainwindow.h"
#include <QtNetwork>
#include <QDebug>
mailer::mailer()
{

}
int mailer::sendEmail(const QString recipientEmail ){

    qDebug()<<"sslLibraryBuildVersionString: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"sslLibraryVersionNumber: "<<QSslSocket::sslLibraryVersionNumber();
     qDebug()<<"supportsSsl: "<<QSslSocket::supportsSsl();;
    qDebug()<<QCoreApplication::libraryPaths();

        QString smtpServer = "smtp.gmail.com";
        int smtpPort = 465;
        QString username = "mariem.khalfallah18102003@gmail.com";
        QString password = "ucah ulwb vtjz dhfa";

        QString from = "mariem.khalfallah18102003@gmail.com";
        //QString recipientEmail=ui->le_destinataire->text();
        QString to = recipientEmail;
        //QString to = "mariem.khalfallah@esprit.tn";
        QString subject = "Encouragement and support";
        QString body = "We appreciate your use of our app to provide commands. Thank you for choosing our platform ,\n we want to inform you about similar products that we have. \nIf you want to make another order you can contact us again. ";


        QSslSocket socket;

        socket.connectToHostEncrypted(smtpServer, smtpPort);
        if (!socket.waitForConnected()) {
            qDebug() << "Error connecting to the server:" << socket.errorString();
            return -1;
        }

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        qDebug() << "Connected to the server.";

        socket.write("HELO localhost\r\n");
        socket.waitForBytesWritten();

        // Read the response from the server
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write("AUTH LOGIN\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write(QByteArray().append(username.toUtf8()).toBase64() + "\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write(QByteArray().append(password.toUtf8()).toBase64() + "\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write("MAIL FROM:<" + from.toUtf8() + ">\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write("RCPT TO:<" + to.toUtf8() + ">\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write("DATA\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write("From: " + from.toUtf8() + "\r\n");
        socket.write("To: " + to.toUtf8() + "\r\n");
        socket.write("Subject: " + subject.toUtf8() + "\r\n");
        socket.write("\r\n");
        socket.write(body.toUtf8() + "\r\n");
        socket.write(".\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        socket.write("QUIT\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        qDebug() << "Email sent successfully.";

        socket.close();
}
