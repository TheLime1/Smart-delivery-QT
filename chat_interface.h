#ifndef CHAT_INTERFACE_H
#define CHAT_INTERFACE_H

#include <QDialog>
#include "chat.h"
#include <QProcess>

namespace Ui
{
    class chat_interface;
}

class chat_interface : public QDialog
{
    Q_OBJECT

public:
    explicit chat_interface(QWidget *parent = nullptr);
    ~chat_interface();

private slots:

    void on_send_button_clicked();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::chat_interface *ui;
    chat ch;
    QProcess process;
};

#endif // CHAT_INTERFACE_H
