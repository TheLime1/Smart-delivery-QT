#ifndef MAILER_H
#define MAILER_H
#include <QString>

class mailer
{
public:
    mailer();
    static int sendEmail(const QString recipientEmail);
};

#endif // MAILER_H

